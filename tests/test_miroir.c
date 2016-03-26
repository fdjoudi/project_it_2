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

int test_automate_accessible(){
	int result = 1;

	{
		Automate * automate = creer_automate();

		ajouter_transition( automate, 1, 'a', 1 );
		ajouter_transition( automate, 1, 'b', 2 );
		ajouter_etat_initial( automate, 1);
		ajouter_etat_final( automate, 2);

		Automate * aut = miroir( automate );

		TEST(
			1
			&& aut
			&& le_mot_est_reconnu( aut, "b" )
			&& le_mot_est_reconnu( aut, "ba" )
			&& le_mot_est_reconnu( aut, "baa" )
			&& le_mot_est_reconnu( aut, "baaa" )
			&& ! le_mot_est_reconnu( aut, "" )
			&& ! le_mot_est_reconnu( aut, "a" )
			&& ! le_mot_est_reconnu( aut, "aa" )
			&& ! le_mot_est_reconnu( aut, "aaa" )
			&& ! le_mot_est_reconnu( aut, "aba" )
			&& ! le_mot_est_reconnu( aut, "bba" )
			, result
		);
		liberer_automate( aut );
		liberer_automate( automate );
	}

	return result;
}


int main(){

	if( ! test_automate_accessible() ){ return 1; };

	return 0;
	
}
