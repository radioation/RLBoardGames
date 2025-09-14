import pytest
import chess

from chess_game import ChessGame


def test_new_game_init():
    g = ChessGame('S', 'W')
    assert isinstance( g.id, str )
    assert g.id != ""
    assert g.id.isalnum() is True
    assert isinstance( g.player_1_id, str )
    assert g.player_1_id != ""
    assert g.player_1_id.isalnum() is True
    assert isinstance( g.player_2_id, str )
    assert g.player_2_id != ""
    assert g.player_2_id.isalnum() is True

    assert g.player_1_id != g.player_2_id

    assert g.mode == 'S'
    assert g.player_1_side == 'W'
    assert g.curr_player == 1

    assert g.board.is_valid() is True
    assert g.board.is_game_over() is False
    assert g.moves == []

def test_new_game_double_black_init():
    g = ChessGame('D', 'B')
    assert g.mode == 'D'
    assert g.player_1_side == 'B'


