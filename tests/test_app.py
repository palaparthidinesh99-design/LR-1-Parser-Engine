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
