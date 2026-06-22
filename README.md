# LR-1-Parser-Engine

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![Python](https://img.shields.io/badge/python-3.10%2B-blue)](https://www.python.org/)
[![Live Demo](https://img.shields.io/badge/demo-online-brightgreen)](https://lr-1-parser-engine.onrender.com/)

LR-1-Parser-Engine is a lightweight parser engine with a Flask web interface for visualizing LR(1) grammar parsing, parse tables, and derivation output.

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Quickstart](#quickstart)
- [Repository Structure](#repository-structure)
- [Examples](#examples)
- [Docker & Deployment](#docker--deployment)
- [Testing](#testing)
- [Notes](#notes)
- [License](#license)

## Features

- Interactive Flask web UI
- LR(1) parser engine compiled from C++ sources
- Visual parse tree and parse table output
- Support for example grammars in `data/examples/`
- Docker-ready deployment with `Dockerfile` and `render.yaml`

## Demo

A live demo is available at:

[https://lr-1-parser-engine.onrender.com/](https://lr-1-parser-engine.onrender.com/)

### Screenshots

![Derivation Tree](docs/images/demo-1.png)

![Action Goto Table](docs/images/demo-2.png)

![Stack Trace](docs/images/demo-3.png)

## Quickstart

```powershell
python -m venv .venv
.venv\Scripts\Activate.ps1
pip install -r requirements.txt
python app.py
```

Then open:

```
http://127.0.0.1:5000
```

## Repository Structure

- `app.py` — Flask application entrypoint
- `src/` — C++ parser source code
- `data/` — grammar/input files and examples
- `templates/` — HTML UI templates
- `docs/` — documentation and screenshots
- `tests/` — automated tests
- `Dockerfile` — Docker image definition
- `render.yaml` — Render deployment manifest

## Examples

The repository includes sample grammar files under `data/examples/`:

- `data/examples/arithmetic_grammar.txt`
- `data/examples/arithmetic_input.txt`
- `data/examples/boolean_grammar.txt`
- `data/examples/boolean_input.txt`

## Docker & Deployment

### Local Docker

Build and run the container locally:

```bash
docker build -t lr1-parser-engine .
docker run -p 5000:5000 lr1-parser-engine
```

### Docker Compose

Run with live code mounting:

```bash
docker-compose up --build
```

### Render Deployment

A Render manifest is included in `render.yaml`.

To deploy on Render:

1. Sign in to https://render.com.
2. Create a new Web Service.
3. Connect the repository `palaparthidinesh99-design/LR-1-Parser-Engine`.
4. Choose Docker as the environment.
5. Confirm branch `main` and deploy.

## Testing

Run automated tests locally:

```powershell
pytest -q
```

## Notes

- `g++` must be available in `PATH` so the parser can compile successfully.
- The Flask app compiles the C++ parser when `/run` is called.

## License

MIT — see `LICENSE`.
