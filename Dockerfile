FROM python:3.11-slim

# Set workdir
WORKDIR /app

# Install build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# Copy requirements first to leverage layer cache
COPY requirements.txt /app/
RUN pip install --no-cache-dir -r requirements.txt

# Copy project files
COPY . /app

# Compile the parser engine once during build
RUN g++ -std=c++11 src/main.cpp src/grammar.cpp src/parser.cpp -o parser.exe

# Expose port and run using gunicorn
EXPOSE 5000
CMD ["gunicorn", "-b", "0.0.0.0:5000", "app:app", "--workers=2"]
