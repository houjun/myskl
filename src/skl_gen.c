
/***************************************************************************
 *   Copyright (C) 2010 by Andrea Ghersi                                   *
 *   ghangenit@users.sourceforge.net                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_RAND
# define _PROTO_PRIVATE_GEN
# define _PROTO_PRIVATE_DBG
# define _PROTO_PUBLIC_GEN

# include "skl_priv.h"


/** **************************************************************
 *                                                               *
 * Genera un numero 'x+1' ( x>= 1 ) con probabilita': <1/2^x> in *
 * modo efficiente - effettua una sola volta la chiamata a rand  *
 *                                                               *
 * Quando t>RAND_MAX/j ci fermiamo e ritorniamo il valore di lev *
 *                                                               *
 * 1) nel 50% dei casi abbiamo t>RAMD_MAX/2, quindi solo nel 50% *
 *    andiamo avanti e ritorniamo lev >= 2;                      *
 *                                                               *
 * 2) nel 75% dei casi abbiamo t>RAND_MAX/4, quindi solo nel 25% *
 *    andiamo avanti e ritorniamo lev >= 3                       *
 *                                                               *
 * 3) nell' 87.5% dei casi abbiamo t>RAND_MAX/8, quindi solo nel *
 *    12,5% andiamo avanti e ritorniamo lev >= 4                 *
 *                                                               *
 * Il 50% e' quindi valido per lev=2 ma non per lev=3. Il 25% e' *
 * valido per lev=3, ma non per lev=4 e via dicendo. In pratica  *
 * si ritorna lev=2 nel 50% dei casi, lev=3 nel 25%,......       *
 *                                                               *
 ** **************************************************************/

# ifdef MYSKL_USE_RAND

uint genLevel ( MySKL_t l )
{
    uint  lev = 1, j = 2;
    uint maxlevel, curlevel;
    ulong t;

    X_ENTER ( THIS_IS_ME );

    maxlevel = l->maxlevel;
    curlevel = l->curlevel;

    /* let's generate a pseudo casual number */
    t = RNGyieldInteger32 ( l->rng_item );

    for ( ; lev < maxlevel; lev++, j <<= 1 /*j *= 2*/ )
        if (
            ( t > RAND_MAX_N / j ) ||
            ( lev > curlevel )
        ) break;

    /* debug call if enabled */
    X_RETURN ( lev );

}

# endif



/** **************************************************************
 *                                                               *
 * Genera il livello da assegnare ad un nodo partendo dal numero *
 * di nodi presenti nella lista (senza considerare il nodo che e'*
 * oggetto dell'inserimento) e calcolando quanti nodi un livello *
 * dovrebbe avere. Se tale livello e' gia' al completo, si passa *
 * a quello successivo. Se anch'esso e' al completo si continua  *
 * a salire di livello e cosi' via.                              *
 *                                                               *
 * Esempio, con z = 100. A livello 2 dobbiamo avere 50 nodi. Se  *
 * guardiamo l'algoritmo a livello 2 permettiamo 100/4 nodi (25) *
 * A livello 3 ne permettiamo 12, mentre ne permettiamo 6 a liv  *
 * 4 e cosi' via. Tutti i nodi di liv. superiore a 2 sono anche  *
 * di livello 2: i nodi totali a livello 2 sono quindi: 25+12+6  *
 * ovvero tendono a 50, come volevamo                            *
 *                                                               *
 ** **************************************************************/

# ifdef MYSKL_NO_RAND

uint genLevel ( MySKL_t l )
{
    uint maxlevel, lev = 1;
    double z;

    X_ENTER ( THIS_IS_ME );

    if ( !l->nodenum ) X_RETURN ( 1 );

    z= (double) l->nodenum;
    maxlevel = l->maxlevel;

    while (1)
    {
        if ( lev == maxlevel ) break;
        z = z/2;

        if ( (double)l->nodenumelem[ lev ] < z ) break;
        lev++;
    }

    l->nodenumelem[ lev ]++;
    X_RETURN ( lev );
}

# endif



# if 0
/** ***********************************************************
 ** ***********************************************************
 +                                                            +
 +                                                            +
 +               FUNZIONI/PROCEDURE PUBBLICHE                 +
 +                                                            +
 +                                                            +
 ** ***********************************************************
 ** ***********************************************************/

uint MySKLgenLevel( MySKL_t l )
{
     X_ENTER ( THIS_IS_ME );

     /* return 0 if list error occurs */
     LDN_BC_X_RETV( l, DSKIP, NSKIP, 0 );

     X_RETURN ( genLevel( l ) );
}
# endif
