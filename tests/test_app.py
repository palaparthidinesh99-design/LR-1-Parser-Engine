from app import app


def test_index_status_code():
    client = app.test_client()
    response = client.get('/')
    assert response.status_code == 200
    assert b'LR(1) Parser Engine' in response.data


def test_run_endpoint_rejects_missing_json():
    client = app.test_client()
    response = client.post('/run')
    assert response.status_code >= 400
    assert response.status_code < 500


def test_run_endpoint_returns_json_error_on_subprocess_failure(monkeypatch):
    def raise_file_not_found(*args, **kwargs):
        raise FileNotFoundError('g++ not found')

    monkeypatch.setattr('app.subprocess.run', raise_file_not_found)

    client = app.test_client()
    response = client.post('/run', json={'grammar': 'S -> a', 'input_string': 'a'})

    assert response.status_code == 200
    data = response.get_json()
    assert 'error' in data
