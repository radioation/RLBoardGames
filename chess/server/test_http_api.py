
import pytest
import json
from rlsf_chess.http_api import app
from rlsf_chess.chess_game import ChessGame


def test_newgame():
    client = app.test_client()

    # create a newgame
    resp = client.post("/newgame", data="S\nW\n")
    assert resp.status_code == 200
    assert len(resp.data) == 18   # plus 
    ids = resp.data.decode("utf-8")
    assert ids[8] == ":"
    assert ids[:8].isalnum() == True
    assert ids[9:-1].isalnum() == True
