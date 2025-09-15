
import pytest
import json
from rlsf_chess.http_api import app
from rlsf_chess.chess_game import ChessGame


def test_newgame_SW():
    client = app.test_client()

    # create a newgame
    resp = client.post("/newgame", data="S\nW\n")
    assert resp.status_code == 200
    assert len(resp.data) == 18   # plus 
    ids = resp.data.decode("utf-8")
    assert ids[8] == ":"
    assert ids[:8].isalnum() == True
    assert ids[9:-1].isalnum() == True

def test_newgame_SW10():
    client = app.test_client()

    # create a newgame
    resp = client.post("/newgame", data="S\nW\n10\n")
    assert resp.status_code == 200
    assert len(resp.data) == 18   # plus 
    ids = resp.data.decode("utf-8")
    assert ids[8] == ":"
    assert ids[:8].isalnum() == True
    assert ids[9:-1].isalnum() == True


def test_joingame():
    client = app.test_client()

    # create a newgame
    resp = client.post("/newgame", data="S\nW\n")
    ids = resp.data.decode("utf-8")
    gameid =  ids[:8]
    playid = ids[9:-1]

    resp = client.post("/joingame")
    assert resp.status_code == 400


    resp = client.post("/joingame", data="AAAAAAAA\n")
    assert resp.status_code == 404

    resp = client.post("/joingame", data=f"{gameid}\n")
    assert resp.status_code == 200

    
def test_move():
    client = app.test_client()

    # check post data
    resp = client.post("/move", data="S\nW\n")
    assert resp.data.decode('utf-8') == "invalid format\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="-----\nW\na\n")
    assert resp.data.decode('utf-8') == "invalid format - g\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="abcd1234\nW\na\n")
    assert resp.data.decode('utf-8') == "invalid format - p\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="abcd1234\n1234asdf\na\n")
    assert resp.data.decode('utf-8') == "invalid format - m\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="abcd1234\n1234asdf\n123456\n")
    assert resp.data.decode('utf-8') == "invalid format - m\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="abcd1234\n1234asdf\ne2e4\n")
    assert resp.data.decode('utf-8') == "invalid game\n"
    assert resp.status_code == 404

    resp = client.post("/move", data="abcd1234\n1234asdf\ne2e4\naaa\n")
    assert resp.data.decode('utf-8') == "invalid format - t\n"
    assert resp.status_code == 400

    resp = client.post("/move", data="abcd1234\n1234asdf\ne2e4\n350\n")
    assert resp.data.decode('utf-8') == "invalid game\n"
    assert resp.status_code == 404


    # create a newgame
    resp = client.post("/newgame", data="S\nW\n")
    ids = resp.data.decode("utf-8")
    gameid =  ids[:8]
    playid = ids[9:-1]

    resp = client.post("/move", data=f"{gameid}\n{playid}\ne2e4\n")
    assert resp.status_code == 200






