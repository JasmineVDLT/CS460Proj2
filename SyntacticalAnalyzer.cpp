/*******************************************************************************
* File name:                                                                   *
* Project: CS 460 Project 2 Fall 2019                                          *
* Author(s):                                                                   *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	token = lex->GetToken();
	int totalErrors = program ();
	string temp = filename;
	temp = temp.substr(0, temp.size() - 3);
	p2.open(temp + ".p2");
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program ()
{
	int errors = 0;
	if(token == LPAREN_T) {
		token = lex->GetToken();
		p2 << "Using Rule 1" << endl;
	}
	else{
		errors++;
		lex->ReportError("Error something missing\n");
	}
	errors += define();
	if(token == LPAREN_T) {
		token = lex->GetToken();
		p2 << "Using Rule 1" << endl;
	} else{
		errors++;
		lex->ReportError("Error something missing\n");
	}
	errors += more_defines();
	if(token == EOF_T) {
		p2 << "Using Rule 1" << endl;
		return errors;
	}
	else{
		errors++;
		lex->ReportError("Error something missing\n");
	}
	p2.close();
	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
	int errors = 0;
	if(token == DEFINE_T) {
		token = lex->GetToken();
		p2 << "Using Rule 2" << endl;
		errors += define();

		if (token == LPAREN_T) {
			token = lex->GetToken();
			p2 << "Using Rule 2" << endl;
		}
		else {
		errors++;
		lex->ReportError("Error something missing\n");
		}
		errors += more_defines();
	}
	else if(token == IDENT_T){
		p2 << "Using Rule 3" << endl;
		token = lex->GetToken();
		errors += stmt_list();
		if(token == RPAREN_T){
			p2 << "Using Rule 3" << endl;
			token = lex->GetToken();
		}
		else {
			errors++;
			lex->ReportError("Error something missing\n");
		}
	}
	else {
		errors++;
		lex->ReportError("Error something missing\n");
	}

	return errors;
}

int SyntacticalAnalyzer::define ()
{
	int errors = 0;
	if(token == DEFINE_T)
	{
		p2 << "Using Rule 4" << endl;
		token = lex->GetToken();
	} else
	{
		errors++;
		lex->ReportError("Error something missing\n");
	}
	if(token == LPAREN_T)
	{
		p2 << "Using Rule 4" << endl;
		token = lex->GetToken();
	}
	else {
		errors++;
		lex->ReportError("Error something missing\n");
	}
	if(token == IDENT_T)
	{
		p2 << "Using Rule 4" << endl;
		token = lex->GetToken();
	} else {
		errors++;
		lex->ReportError("Error something missing\n");
	}
	errors += param_list();

	if(token == RPAREN_T)
	{
		p2 << "Using Rule 4" << endl;
		token = lex->GetToken();

	} else
	{
		errors++;
		lex->ReportError("Error something missing\n");
	}

	errors += stmt();

	errors += stmt_list();

	if(token == RPAREN_T)
	{
		p2 << "Using Rule 4" << endl;
		token = lex->GetToken();
		return errors;
	} else{
		errors++;
		lex->ReportError("Error something missing\n");
		return errors;
	}

}

int SyntacticalAnalyzer::stmt_list () {
	int errors = 0;

	if(token == RPAREN_T || token == IDENT_T || token == LPAREN_T || token == NUMLIT_T
	    || token == STRLIT_T || token == SQUOTE_T || token == IDENT_T)
	{
		p2 << "Using Rule 6" << endl;
		return errors;
	}

	else {
	errors += stmt();
	errors += stmt_list();
	}
	return errors;
}

int SyntacticalAnalyzer::stmt ()
{
	int errors = 0;

	if(token == IDENT_T) {
		p2 << "Using Rule 8" << endl;
		token = lex->GetToken();
		return errors;
	}
	else if(token == LPAREN_T) {
		p2 << "Using Rule 8" << endl;
		token = lex->GetToken();
		errors += action();
		if(token == RPAREN_T) {
			p2 << "Using Rule 8" << endl;
			token = lex->GetToken();
			return errors;
		}
	}

	errors += literal();
	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	if(token == NUMLIT_T) {
		p2 << "Using Rule 10" << endl;
		token = lex->GetToken();
		return errors;
	}
	else if(token == STRLIT_T) {
		p2 << "Using Rule 11" << endl;
		token = lex->GetToken();
		return errors;
	}
	else if(token == SQUOTE_T)
	{
		p2 << "Using Rule 12" << endl;
		token = lex->GetToken();
		errors += quoted_lit();
	}
	return errors;
}

int SyntacticalAnalyzer::quoted_lit ()
{
	int errors = 0;
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == LISTOP2_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T
     		|| token == LISTOP1_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T
     		|| token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T
     		|| token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T){
    		p2 << "Using Rule 13\n";
    		errors += any_other_token();
  	}
  	else{
    		errors++;
    		lex->ReportError("Error, missing something");
    		token = lex->GetToken();
  	}

	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == LISTOP2_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T
     		|| token == LISTOP1_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T
     		|| token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T
     		|| token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == SQUOTE_T || token == COND_T || token == ELSE_T){
    		p2 << "Using Rule 14\n";
    		errors += any_other_token();
    		errors += more_tokens();
  	}
  	else if(token == LAMBDA){
    		p2 << "Using Rule 15\n";
    		token = lex->GetToken();
  	}
  	else{
    		errors++;
    		lex->ReportError("Error, missing something");
    		token = lex->GetToken();
  	}

	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
	int errors = 0;
	if(token == IDENT_T){
    		p2 << "Using Rule 16\n";
    		token = lex->GetToken();
    		errors += param_list();
  	}
  	else if(token == RPAREN_T || token == LPAREN_T){
    		p2 << "Using Rule 17\n";
  	}
  	else{
    		errors++;
    		lex->ReportError("Error, missing identifier");
    		token = lex->GetToken();
  	}

	return errors;
}

int SyntacticalAnalyzer::else_part ()
{
	int errors = 0;
	if(token == RPAREN_T || token == IDENT_T || token == LPAREN_T
		|| token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		token = lex->GetToken();
		p2 << "Using Rule 24" << endl;
		return errors;
	}
	else {
		token = lex->GetToken();
		errors += stmt();
	}


	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	if(token == LPAREN_T)
	{
		token = lex->GetToken();
		p2 << "Using Rule 20" << endl;
		errors += stmt_pair_body();
	}
	else if (token == RPAREN_T || token == IDENT_T || token == LPAREN_T || token == NUMLIT_T
			  || token == STRLIT_T || token == SQUOTE_T)
	{
		token = lex->GetToken();
		p2 << "Using Rule 21" << endl;
	}
	return errors;
}

int SyntacticalAnalyzer::stmt_pair_body ()
{
	int errors = 0;
	if(token == ELSE_T)
	{
		token = lex->GetToken();
		p2 << "Using Rule 23" << endl;
		errors += stmt();
		if(token == RPAREN_T)
		{
			p2 << "Using Rule 23" << endl;
			token = lex->GetToken();

		}
		errors += stmt_pair();
	}
	else {
		errors += stmt();
		errors += stmt();
		if(token == RPAREN_T)
		{
			p2 << "Using Rule 22" << endl;
			token = lex->GetToken();
		}
	}

	return errors;
}

int SyntacticalAnalyzer::action ()
{
	int errors = 0;
	if(token == IF_T){
    		p2 << "Using Rule 24\n";
    		token = lex->GetToken();
    		errors += stmt();
    		errors += stmt();
    		errors += else_part();
  	}
  	else if(token == COND_T){
    		p2 << "Using Rule 25\n";
    		token = lex->GetToken();
    		if(token == LPAREN_T){
      			p2 << "Using Rule 25\n";
      			token = lex->GetToken();
      			errors += stmt_pair_body();
    		}
    		else{
      			errors++;
      			lex->ReportError("Error, missing (");
      			token = lex->GetToken();
    		}
  	}
  	else if(token == LISTOP1_T){
    		p2 << "Using Rule 26\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == LISTOP2_T){
    		p2 << "Using Rule 27\n";
    		token = lex->GetToken();
    		errors += stmt();
    		errors += stmt();
  	}
  	else if(token == AND_T){
    		p2 << "Using Rule 28\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == OR_T){
    		p2 << "Using Rule 29\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == NOT_T){
    		p2 << "Using Rule 30\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == NUMBERP_T){
    		p2 << "Using Rule 31\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == LISTP_T){
    		p2 << "Using Rule 32\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == ZEROP_T){
    		p2 << "Using Rule 33\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
	else if(token == NULLP_T){
    		p2 << "Using Rule 34\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == STRINGP_T){
    		p2 << "Using Rule 35\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == PLUS_T){
    		p2 << "Using Rule 36\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == MINUS_T){
    		p2 << "Using Rule 37\n";
    		token = lex->GetToken();
    		errors += stmt();
    		errors += stmt_list();
  	}
  	else if(token == DIV_T){
    		p2 << "Using Rule 38\n";
    		token = lex->GetToken();
    		errors += stmt();
    		errors += stmt_list();
  	}
  	else if(token == MULT_T){
    		p2 << "Using Rule 39\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
	else if(token == MODULO_T){
		p2 << "Using Rule 40\n";
    		token = lex->GetToken();
    		errors += stmt();
    		errors += stmt();
  	}
  	else if(token == ROUND_T){
    		p2 << "Using Rule 41\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == EQUALTO_T){
    		p2 << "Using Rule 42\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == GT_T){
    		p2 << "Using Rule 43\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == LT_T){
    		p2 << "Using Rule 44\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == GTE_T){
    		p2 << "Using Rule 45\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == LTE_T){
    		p2 << "Using Rule 46\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == IDENT_T){
    		p2 << "Using Rule 47\n";
    		token = lex->GetToken();
    		errors += stmt_list();
  	}
  	else if(token == DISPLAY_T){
    		p2 << "Using Rule 48\n";
    		token = lex->GetToken();
    		errors += stmt();
  	}
  	else if(token == NEWLINE_T){
    		p2 << "Using Rule 49\n";
    		token = lex->GetToken();
  	}
  	else{
    		errors++;
    		lex->ReportError("Error, missing somthing");
    		token = lex->GetToken();
  	}

	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	if(token == LPAREN_T)
	{
		p2 << "Using Rule 50" << endl;
		token = lex->GetToken();
		errors += more_tokens();
		if(token == RPAREN_T)
		{
			token = lex->GetToken();
			return errors;
		}
	}
	else if(token == IDENT_T)
	{
		p2 << "Using Rule 51" << endl;
		token = lex->GetToken();
	}
	else if(token == NUMLIT_T)
	{
		p2 << "Using Rule 52" << endl;
		token = lex->GetToken();
	}
	else if(token == STRLIT_T)
	{
		p2 << "Using Rule 53" << endl;
		token = lex->GetToken();
	}
	else if(token == LISTOP2_T)
	{
		p2 << "Using Rule 54" << endl;
		token = lex->GetToken();
	}
	else if(token == IF_T)
	{
		p2 << "Using Rule 55" << endl;
		token = lex->GetToken();
	}
	else if(token == DISPLAY_T)
	{
		p2 << "Using Rule 56" << endl;
		token = lex->GetToken();
	}
	else if(token == NEWLINE_T)
	{
		p2 << "Using Rule 57" << endl;
		token = lex->GetToken();
	}
	else if(token == LISTOP1_T)
	{
		p2 << "Using Rule 58" << endl;
		token = lex->GetToken();
	}
	else if(token == AND_T)
	{
		p2 << "Using Rule 59" << endl;
		token = lex->GetToken();
	}
	else if(token == OR_T)
	{
		p2 << "Using Rule 60" << endl;
		token = lex->GetToken();
	}
	else if(token == NOT_T)
	{
		p2 << "Using Rule 61" << endl;
		token = lex->GetToken();
	}
	else if(token == DEFINE_T)
	{
		p2 << "Using Rule 62" << endl;
		token = lex->GetToken();
	}
	else if(token == NUMBERP_T)
	{
		p2 << "Using Rule 63" << endl;
		token = lex->GetToken();
	}
	else if(token == LISTP_T)
	{
		p2 << "Using Rule 64" << endl;
		token = lex->GetToken();
	}
	else if(token == ZEROP_T)
	{
		p2 << "Using Rule 65" << endl;
		token = lex->GetToken();
	}else if(token == NULLP_T)
	{
		p2 << "Using Rule 66" << endl;
		token = lex->GetToken();
	}
	else if(token == STRINGP_T)
	{
		p2 << "Using Rule 67" << endl;
		token = lex->GetToken();
	}
	else if(token == PLUS_T)
	{
		p2 << "Using Rule 68" << endl;
		token = lex->GetToken();
	}
	else if(token == MINUS_T)
	{
		p2 << "Using Rule 69" << endl;
		token = lex->GetToken();
	}
	else if(token == DIV_T)
	{
		p2 << "Using Rule 70" << endl;
		token = lex->GetToken();
	}
	else if(token == MULT_T)
	{
		p2 << "Using Rule 71" << endl;
		token = lex->GetToken();
	}
	else if(token == MODULO_T)
	{
		p2 << "Using Rule 72" << endl;
		token = lex->GetToken();
	}
	else if(token == ROUND_T)
	{
		p2 << "Using Rule 73" << endl;
		token = lex->GetToken();
	}
	else if(token == EQUALTO_T)
	{
		p2 << "Using Rule 74" << endl;
		token = lex->GetToken();
	}
	else if(token == GT_T)
	{
		p2 << "Using Rule 75" << endl;
		token = lex->GetToken();
	}
	else if(token == LT_T)
	{
		p2 << "Using Rule 76" << endl;
		token = lex->GetToken();
	}
	else if(token == GTE_T)
	{
		p2 << "Using Rule 77" << endl;
		token = lex->GetToken();
	}
	else if(token == LTE_T)
	{
		p2 << "Using Rule 78" << endl;
		token = lex->GetToken();
	}
	else if(token == SQUOTE_T)
	{
		p2 << "Using Rule 79" << endl;
		token = lex->GetToken();
		errors += any_other_token();
	}
	else if(token == COND_T)
	{
		p2 << "Using Rule 80" << endl;
		token = lex->GetToken();
	}
	else if(token == ELSE_T)
	{
		p2 << "Using Rule 81" << endl;
		token = lex->GetToken();
	}

	return errors;
}

int SyntacticalAnalyzer::parse_file() {
	return program();
}

