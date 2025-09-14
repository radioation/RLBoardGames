import pytest
import chess

from chess_game import ChessGame


def test_new_game_init():
    g = ChessGame('S', 'W')
    assert isinstance( g.id, str )
    assert g.id != ""
    assert isinstance( g.player_1_id, str )
    assert isinstance( g.player_2_id, str )
    assert g.mode == 'S'
    assert g.player_1_side == 'W'
    assert g.curr_player == 1

    assert g.board.is_valid() is True
    assert g.board.is_game_over() is False
    assert g.moves == []
