import os
import subprocess
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route('/')
def index():
    # Read the default grammar file to pre-fill the textbox
    grammar_content = ""
    if os.path.exists('data/grammar.txt'):
        with open('data/grammar.txt', 'r') as f:
            grammar_content = f.read()
            
    input_content = "id + id * id $"
    if os.path.exists('data/input.txt'):
        with open('data/input.txt', 'r') as f:
            input_content = f.read()
            
    return render_template('index.html', grammar=grammar_content, initial_input=input_content)

@app.route('/run', methods=['POST'])
def run_parser():
    try:
        data = request.json
        grammar_text = data.get('grammar', '')
        input_text = data.get('input_string', 'id + id * id $')
        
        # Save the grammar and input to file
        os.makedirs('data', exist_ok=True)
        with open('data/grammar.txt', 'w') as f:
            f.write(grammar_text)
        with open('data/input.txt', 'w') as f:
            f.write(input_text)
            
        # Rebuild and run the parser to ensure it's up to date
        compile_cmd = ['g++', '-std=c++11', 'src/main.cpp', 'src/grammar.cpp', 'src/parser.cpp', '-o', 'parser.exe']
        compile_proc = subprocess.run(compile_cmd, capture_output=True, text=True)
        if compile_proc.returncode != 0:
            return jsonify({'error': 'Compilation failed:\n' + compile_proc.stderr})
            
        # Run the executable
        executable = './parser.exe' if os.name != 'nt' else 'parser.exe'
        proc = subprocess.run([executable], capture_output=True, text=True)
        if proc.returncode != 0:
            return jsonify({'error': 'Execution failed:\n' + proc.stderr})
            
        output = proc.stdout
        
        # Split the output simply
        import re
        sections = re.split(r'\n---\s*([A-Za-z0-ALS\s]+)\s*---\n', output)
        
        parsed_sections = {'summary': sections[0].strip()}
        
        for i in range(1, len(sections), 2):
            if i+1 < len(sections):
                name = sections[i].strip()
                content = sections[i+1].strip()
                parsed_sections[name] = content
                
        return jsonify({'success': True, 'raw': output, 'sections': parsed_sections})
    except Exception as e:
        import traceback
        return jsonify({'error': f'Server exception: {str(e)}\n{traceback.format_exc()}'})

if __name__ == '__main__':
    app.run(debug=True, port=5000)
