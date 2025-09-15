import pytest
import chess

from chess_game import ChessGame


def test_new_game_init():
    g = ChessGame()
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

    assert g.skill_level == 3
    assert g.engine_config == {"UCI_LimitStrength": True, "UCI_Elo" : 1735 } 

def test_new_game_w_settings_init():
    g = ChessGame( 'S', 'B', 10 )
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
    assert g.player_1_side == 'B'
    assert g.curr_player == 1

    assert g.board.is_valid() is True
    assert g.board.is_game_over() is False
    assert g.moves == []

    assert g.skill_level == 10 
    assert g.engine_config == {"UCI_LimitStrength": False }

def test_new_game_double_init():
    g = ChessGame('D', 'W')
    assert g.mode == 'D'
    assert g.player_1_side == 'W'
    assert g.curr_player == 1

def test_new_game_double_black_init():
    g = ChessGame('D', 'B')
    assert g.mode == 'D'
    assert g.player_1_side == 'B'
    assert g.curr_player == 2

 
def test_legal_move():
    g = ChessGame('S', 'W')
    # current player should be 1
    p = g.player_1_id
    resp = g.do_move( p, "e2e4", 300)
    assert resp != 'none'
    assert resp != 'illegal move'
    assert len(resp) == 4
    assert resp.isalnum() is True
    assert len(g.board.move_stack) == 2 # two moves because chess engine is applied in single player mode.

def test_illegal_move():
    g = ChessGame('S', 'W')
    # current player should be 1
    p = g.player_1_id
    resp = g.do_move( p, "e2e5", 300)
    assert resp == 'illegal move'
    assert len(g.board.move_stack) == 0 


def test_player_1W_turns():
    g = ChessGame('D', 'W')
    # current player should be 1
    p1 = g.player_1_id
    p2 = g.player_2_id
    resp = g.do_move( p2, "e2e4", 300)
    assert resp == 'illegal move: player 1 turn'
    assert g.curr_player == 1
    assert len(g.board.move_stack) == 0  # no valid moves yet, 0

    resp = g.do_move( p1, "e2e4", 300)
    assert g.curr_player == 2
    assert len(g.board.move_stack) == 1 # one valid move and chess engine is NOT applied in doubles mode.

    resp = g.do_move( p1, "e7e5", 300)
    assert resp == 'illegal move: player 2 turn'
    assert len(g.board.move_stack) == 1 # one valid move

    resp = g.do_move( p2, "e7e5", 300)
    assert resp != 'none'
    assert resp != 'illegal move'
    assert len(resp) == 4
    assert len(g.board.move_stack) == 2 # two valid moves
    assert resp.isalnum() is True

def test_player_1B_turns():
    g = ChessGame('D', 'B')
    # current player should be 1
    assert g.curr_player == 2
    p1 = g.player_1_id
    p2 = g.player_2_id


    resp = g.do_move( p1, "e2e4", 300)
    assert resp == 'illegal move: player 2 turn'
    assert len(g.board.move_stack) == 0  # no valid moves yet, 0

    resp = g.do_move( p2, "e2e4", 300)
    assert g.curr_player == 1
    assert len(g.board.move_stack) == 1 # one valid move and chess engine is NOT applied in doubles mode.

    resp = g.do_move( p2, "e7e5", 300)
    assert resp == 'illegal move: player 1 turn'
    assert len(g.board.move_stack) == 1 # one valid move

    resp = g.do_move( p1, "e7e5", 300)
    assert resp != 'none'
    assert resp != 'illegal move'
    assert len(resp) == 4
    assert len(g.board.move_stack) == 2 # two valid moves
    assert resp.isalnum() is True







