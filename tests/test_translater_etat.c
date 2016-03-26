/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2015 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "automate.h"
#include "outils.h"
#include <limits.h>

#include "ensemble.h"

#include <stdarg.h>

int sont_dans_l_ensemble( int n, Ensemble * ens, ...  ){
	va_list pile;
	int res = 1;
	int i;
	va_start(pile,ens);
	for (i=0;i<n;i++){
		res &= est_dans_l_ensemble( ens, va_arg(pile,int) );
	}
	va_end(pile);
	return res;
}

int l_ensemble_est_egal( int n, const Ensemble* ens, ... ){
	va_list pile;
	int res;
	int i;

	Ensemble * e = creer_ensemble( NULL, NULL, NULL );

	va_start(pile,ens);
	for (i=0;i<n;i++){
		ajouter_element( e, va_arg(pile,int) ); 
 	}

	res = comparer_ensemble( ens, e );
	liberer_ensemble( e );

	va_end(pile);
	return res == 0;
}

void action_test_transitions_automate(
	int origine, char lettre, int fin, void* data
){
	int * res = (int*) data;
	(*res) += 1;
}

int test_transitions_automate( int nb_transitions, const Automate* aut, ... ){
	va_list pile;
	int i;
	int nb = 0;
	pour_toute_transition( aut, action_test_transitions_automate, &nb );
	if( nb_transitions != nb ){ return 0; }

	va_start(pile,aut);
	for (i=0;i<nb_transitions;i++){
		int origine = va_arg(pile,int);
		char lettre = (char) va_arg(pile,int);
		int fin = va_arg(pile,int);
		if(
			! est_une_transition_de_l_automate(
				aut, origine, lettre, fin 
			)
		){ return 0; }
 	}
	
	va_end(pile);
	return 1;
}




int test_get_min_max_etat(){
	int result = 1;


	{
		Automate * automate = creer_automate();

		TEST(
			1
			&& automate
			&& get_max_etat( automate ) == INT_MIN
			&& get_min_etat( automate ) == INT_MAX
			, result
		);
		liberer_automate( automate );
	}


	{
		Automate * automate = creer_automate();

		ajouter_transition( automate, 1, 'a', 2 );
		ajouter_transition( automate, 1, 'b', 3 );
		ajouter_transition( automate, 4, 'c', 3 );
		ajouter_transition( automate, -5, 'd', 6 );
		ajouter_transition( automate, 7, 'e', 8 );
		ajouter_transition( automate, 4, 'f', 2 );
		ajouter_etat_initial( automate, 1);
		ajouter_etat_initial( automate, -5);
		ajouter_etat_initial( automate, 9);
		ajouter_etat_final( automate, 3);
		ajouter_etat_final( automate, 8);
		ajouter_etat_final( automate, 9);

		TEST(
			1
			&& automate
			&& get_max_etat( automate ) == 9
			&& get_min_etat( automate ) == -5
			, result
		);
		liberer_automate( automate );
	}

	return result;
}


int test_translater_automate_entier(){
	int result = 1;

	{
		Automate * automate = creer_automate();
		Automate * trans = translater_automate_entier( automate, 3 );

		TEST(
			1
			&& trans
			&& taille_ensemble( get_etats( trans ) ) == 0
			&& taille_ensemble( get_initiaux( trans ) ) == 0
			&& taille_ensemble( get_finaux( trans ) ) == 0
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate );
	}

	{
		Automate * automate = creer_automate();
		ajouter_transition( automate, -3, 'a', 5 );
		ajouter_etat_initial( automate, -3);
		ajouter_etat_initial( automate, 0);
		ajouter_etat_final( automate, 5);
		ajouter_etat_final( automate, 7);

		Automate * trans = translater_automate_entier( automate, 2 );

		TEST(
			1
			&& trans
			&& l_ensemble_est_egal( 4, get_etats( trans ), -1, 2, 7, 9)
			&& l_ensemble_est_egal( 2, get_initiaux( trans ), -1, 2)
			&& l_ensemble_est_egal( 2, get_finaux( trans ), 7, 9)
			&& test_transitions_automate( 
				1, trans,
				-1, 'a', 7
			)
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate );
	}

	return result;
}


int test_translater_automate(){
	int result = 1;

	{
		Automate * automate = creer_automate();
		Automate * automate_eviter = creer_automate();
		Automate * trans = translater_automate( automate, automate_eviter );

		TEST(
			1
			&& trans
			&& taille_ensemble( get_etats( trans ) ) == 0
			&& taille_ensemble( get_initiaux( trans ) ) == 0
			&& taille_ensemble( get_finaux( trans ) ) == 0
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate_eviter );
		liberer_automate( automate );
	}

	{
		Automate * automate = creer_automate();
		ajouter_transition( automate, -3, 'a', 5 );
		ajouter_etat_initial( automate, -3);
		ajouter_etat_initial( automate, 0);
		ajouter_etat_final( automate, 5);
		ajouter_etat_final( automate, 7);

		Automate * automate_eviter = creer_automate();
		Automate * trans = translater_automate( automate, automate_eviter );

		TEST(
			1
			&& trans
			&& l_ensemble_est_egal( 4, get_etats( trans ), -3, 0, 5, 7)
			&& l_ensemble_est_egal( 2, get_initiaux( trans ), -3, 0)
			&& l_ensemble_est_egal( 2, get_finaux( trans ), 5, 7)
			&& test_transitions_automate( 
				1, trans,
				-3, 'a', 5
			)
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate_eviter );
		liberer_automate( automate );
	}

	{
		Automate * automate = creer_automate();

		Automate * automate_eviter = creer_automate();
		ajouter_transition( automate_eviter, -3, 'a', 5 );
		ajouter_etat_initial( automate_eviter, -3);
		ajouter_etat_initial( automate_eviter, 0);
		ajouter_etat_final( automate_eviter, 5);
		ajouter_etat_final( automate_eviter, 7);

		Automate * trans = translater_automate( automate, automate_eviter );

		TEST(
			1
			&& trans
			&& taille_ensemble( get_etats( trans ) ) == 0
			&& taille_ensemble( get_initiaux( trans ) ) == 0
			&& taille_ensemble( get_finaux( trans ) ) == 0
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate_eviter );
		liberer_automate( automate );
	}

	{
		Automate * automate = creer_automate();
		ajouter_transition( automate, -3, 'a', 5 );
		ajouter_etat_initial( automate, -3);
		ajouter_etat_initial( automate, 0);
		ajouter_etat_final( automate, 5);
		ajouter_etat_final( automate, 7);

		Automate * automate_eviter = creer_automate();
		ajouter_transition( automate_eviter, -3, 'a', 5 );
		ajouter_etat_initial( automate_eviter, -3);
		ajouter_etat_initial( automate_eviter, 0);
		ajouter_etat_final( automate_eviter, 5);
		ajouter_etat_final( automate_eviter, 7);
		Automate * trans = translater_automate( automate, automate_eviter );

		TEST(
			1
			&& trans
			&& l_ensemble_est_egal( 4, get_etats( trans ), 8, 11, 16, 18)
			&& l_ensemble_est_egal( 2, get_initiaux( trans ), 8, 11)
			&& l_ensemble_est_egal( 2, get_finaux( trans ), 16, 18)
			&& test_transitions_automate( 
				1, trans,
				8, 'a', 16
			)
			, result
		);
		liberer_automate( trans );
		liberer_automate( automate_eviter );
		liberer_automate( automate );
	}

	return result;
}


int main(){

	if( ! test_get_min_max_etat() ){ return 1; }
	if( ! test_translater_automate_entier() ){ return 1; }
	if( ! test_translater_automate() ){ return 1; }

	return 0;
}
