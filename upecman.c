/***************************************************************************
 *   upecman.c                                Version 1.1                  *
 *                                                                         *
 *   Pacman Ncurses                                                        *
 *   Copyright (C) 2016         by upe-blink                               *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
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
 ***************************************************************************
 *   To contact the author, please write to:                               *
 *   UPE-blink                                                             *
 *   Email: blinkdaupe@gmail.com                                           *
 *   Webpage: https://github.com/BecoSystems/upe-blink                     *
 *   Phone: +55 (81) 3184-7555                                             *
 ***************************************************************************/

/* ---------------------------------------------------------------------- */
/**
 * @file upecman.c
 * @ingroup upe-blink
 * @brief Pacman Ncurses
 * @only the pacman game in c language with ncurses library
 * @version 20171211.1.1
 * @date 2017-12-11
 * @author upe-blink <<blinkdaupe@gmail.com>>
 * @par Webpage
 * <<a href="https://www.github.com/BecoSystems/upe-blink">
 * @copyright (c) 2016 GNU GPL v2
 * @note This program is free software: you can redistribute it
 * and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation version 2 of the License.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA. 02111-1307, USA.
 * Or read it online at <<http://www.gnu.org/licenses/>>.
 */

/*
 * Instrucoes para compilar:
 *      $make
 * ou
 *      $gcc upecman.c -o upecman.x -Wall -lncurses -g -Og
 *          -Wextra -ansi -pedantic-errors -DDEBUG=1
 */

/* ---------------------------------------------------------------------- */
/* includes */

#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Miscellaneous functions (rand, malloc, srand)*/
#include <ncurses.h> /* Screen handling and optimisation functions */
#include <getopt.h> /* get options from system argc/argv */
#include <string.h> /* Strings functions definitions */
#include <assert.h> /* Verify assumptions with assert */
#include <unistd.h> /* UNIX standard function */
#include "upecman.h" /* To be created for this template if needed */
#include <time.h> /* Time and date functions */
#include <math.h> /* Mathematics functions */
/* #include <dlfcn.h> */ /* Dynamic library */
/* #include <malloc.h> */ /* Dynamic memory allocation */
/* #include <limits.h> */ /* Various C limits */
/* #include <ctype.h> */ /* Character functions */
/* #include <errno.h> */ /* Error number codes errno */
/* #include <signal.h> */ /* Signal processing */
/* #include <stdarg.h> */ /* Functions with variable arguments */
/* #include <pthread.h> */ /* Parallel programming with threads */
/* #include <fcntl.h> */ /* File control definitions */
/* #include <termios.h> */ /* Terminal I/O definitions POSIX */
/* #include <sys/stat.h> */ /* File status and information */
/* #include <float.h> */ /* Float constants to help portability */
/* #include <setjmp.h> */ /* Bypass standard function calls and return */
/* #include <stddef.h> */ /* Various types and MACROS */
/* #include <SWI-Prolog.h> */ /* Prolog integration with C */
/* #include <allegro.h> */ /* A game library for graphics, sounds, etc. */
/* #include <libintl.h> */ /* Internationalization / translation */
/* #include <locale.h> */ /* MACROS LC_ for location specific settings */
/* #include "libeco.h" */ /* I/O, Math, Sound, Color, Portable Linux/Windows */
/* ---------------------------------------------------------------------- */
#define MAIORDIST 30
/** 
 * @ingroup upe-blink
 * @brief This is the main function
 * @main function responsible for starting the game
 *
 * @param[in] argc Argument counter
 * @param[in] argv Argument strings (argument values)
 *
 * @retval 0 If succeed (EXIT_SUCCESS).
 * @retval 1 Or another error code if failed.
 *
 * @code
 *    $./upecman -h
 * @endcode
 *
 * @warning   Be carefull with the ghosts
 * @bug There is a bug with pacman drive
 * @todo Need to do a function responsible for restarting the game
 * @note You can read more about it at <<a href="https://github.com/BecoSystems/upe-blink">
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 *
 */
int main(int argc, char *argv[])
{  
    IFDEBUG("main()\n");
    int opt; /* return from getopt() */
    t_game g; /* the game */
    const char *sready = "Are you ready?(Press <y> or <KEY_LEFT> to 'YES'/<n> or <KEY_RIGHT> to 'NO')"; /* ready string */
    int kin; /* keyboard input */
    int jog = 1; /* criterio de parada do jogo  */
    int i;
    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h  help
     *        -c  copyright & version
     *        -v  verbose
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "hvc")) != EOF)
        switch(opt)
        {
            case 'h':
                help();
                break;
            case 'v':
                verb++;
                break;
            case 'c':
                copyr();
                break;
            case '?':
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    if(verb)
        printf("Verbose level set at: %d\n", verb);

    /* ...and we are done */
    IFDEBUG("Starting the game now...\n");

    g = upecman_init();

    printlab(g);

    mvprintw(10, 25, "%s", sready);
    refresh();

    for(i=blinky;i<=clyde;i++)
        g.ghost[i].tInicio=time(NULL);


    while(1)
    {
        kin = getch();
        if(kin == 'y' || kin == KEY_LEFT) /*iniciar jogo*/
            /* laco para rodar o jogo */
            while(1)
            {
                while((kin != 'q')&&(jog==1))
                {
                    time(&g.pacman.tInicio);
                    kin = getch();    /*captura de tecla*/
                    g = andap(g, kin); /*movimentacao do pacman*/
                    g = fant(g);      /*movimentacao dos fantasmas*/
                    g = score(g);     /*score do jogo*/
                    g = passfase(g);     /*analisa para mudar de fase*/
                    jog=breakgame(g);
                    printlab(g);      /*print do jogo*/
                    usleep(200000);   /*tempo de descanso do processador antes de rodar laco novamente*/
                    time(&g.pacman.tFim);
                    g.pacman.tDecorrido+=difftime(g.pacman.tFim,g.pacman.tInicio);
                }
                while(kin!='n')
                {
                    kin = getch();    /*captura de tecla*/
                    printover(g);

                    if(kin == 'y')
                    {
                        g=upecman_init();
                        jog=1;
                        break;
                    }    
                    usleep(20000);
                }
                if(kin=='n')
                    break;
            }         
        if(kin == 'n' || kin == KEY_RIGHT) /*nao inciar e sair do jogo */
            break;
    } 
    endwin();
    return EXIT_SUCCESS;
} 

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief Prints help information and exit
 * @Responsible for helping the user if he could use the program version
 * @return Void
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 *
 */

void help(void)
{
    IFDEBUG("help()");
    printf("%s - %s\n", "upecman", "Pacman Ncurses");
    printf("\nUsage: ./%s [-h|-v|-c]\n", "upecman");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --copyright, --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    /* add more options here */
    printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    printf("\nTodo:\n\tLong options not implemented yet.\n");
    printf("\nAuthor:\n\tWritten by %s <%s>\n\n", "Ruben Carlo Benante", "rcb@beco.cc");
    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief Prints version and copyright information and exit
 * @function responsible for detailing the program version, as well as printing the copyright and the way out
 * @return Void
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 *
 */

void copyr(void)
{
    IFDEBUG("copyr()");
    printf("%s - Version %s\n", "upecman", VERSION);
    printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 <http://gnu.org/licenses/gpl.html>. This  is  free  software:  you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2016, "Ruben Carlo Benante", "rcb@beco.cc");
    if(verb > 3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function responsible for handling pacman
 * @function responsible for all mechanics behind the movement of pacman, it will also include pacman teleportation in specific places
 * @return t_game
 * @author upe-blink
 * @version 20171112.1.1
 * @date 2017-12-11
 *
 */

t_game andap(t_game g, int kin)
{
    int px, py;

    px = g.pacman.pos.x;
    py = g.pacman.pos.y;

    /*Set direcao*/
    /*mudar direcao para esquerda*/
    if(kin == KEY_LEFT)
        g.pacman.tenta = 1;
    /*mudar direcao para direita*/
    else
        if(kin == KEY_RIGHT)
            g.pacman.tenta = 3;
    /*mudar direcao para cima*/
        else
            if(kin == KEY_UP)
                g.pacman.tenta = 0;
    /*mudar direcao para baixo*/
            else
                if(kin == KEY_DOWN)
                    g.pacman.tenta = 2;

    if(g.pacman.dir == g.pacman.tenta)
    {
        /*movimentacao do pacman para a esquerda*/
        if((g.pacman.dir == left) && (g.lab[py][px - 1] != '#'))
            g.pacman.pos.x--;
        /*movimentacao do pacman para a direita*/
        if((g.pacman.dir == right) && (g.lab[py][px + 1] != '#'))
            g.pacman.pos.x++;
        /*movimentacao do pacman para cima*/
        if((g.pacman.dir == up) && (g.lab[py - 1][px] != '#'))
            g.pacman.pos.y--;
        /*movimentacao do pacman para baixo*/
        if((g.pacman.dir == down) && (g.lab[py + 1][px] != '#') && (g.lab[py + 1][px] != '-'))
            g.pacman.pos.y++;
    }
    else
        if(g.pacman.tenta == 1)
        {
            if(g.lab[py][px - 1] != '#')
            {
                g.pacman.dir = left;
                g.pacman.pos.x--;
            }
            else
            {
                if((g.pacman.dir == right) && (g.lab[py][px + 1] != '#'))
                    g.pacman.pos.x++;
                else
                    if((g.pacman.dir == up) && (g.lab[py - 1][px] != '#'))
                        g.pacman.pos.y--;
                    else
                        if((g.pacman.dir == down) && (g.lab[py + 1][px] != '#') && (g.lab[py + 1][px] != '-'))
                            g.pacman.pos.y++;
            }
        }
        else
            if(g.pacman.tenta == 3)
            {
                if(g.lab[py][px + 1] != '#')
                {
                    g.pacman.dir = right;
                    g.pacman.pos.x++;
                }
                else
                {
                    if((g.pacman.dir == left) && (g.lab[py][px - 1] != '#'))
                        g.pacman.pos.x--;
                    else
                        if((g.pacman.dir == up) && (g.lab[py - 1][px] != '#'))
                            g.pacman.pos.y--;
                        else
                            if((g.pacman.dir == down) && (g.lab[py + 1][px] != '#') && (g.lab[py + 1][px] != '-'))
                                g.pacman.pos.y++;
                }
            }
            else
                if(g.pacman.tenta == 0)
                {
                    if(g.lab[py - 1][px] != '#')
                    {
                        g.pacman.dir = up;
                        g.pacman.pos.y--;
                    }
                    else
                    {
                        if((g.pacman.dir == left) && (g.lab[py][px - 1] != '#'))
                            g.pacman.pos.x--;
                        else
                            if((g.pacman.dir == right) && (g.lab[py][px + 1] != '#'))
                                g.pacman.pos.x++;
                            else
                                if((g.pacman.dir == down) && (g.lab[py + 1][px] != '#') && (g.lab[py + 1][px] != '-'))
                                    g.pacman.pos.y++;
                    }
                }
                else
                    if(g.pacman.tenta == 2)
                    {
                        if((g.lab[py + 1][px] != '#') && (g.lab[py + 1][px] != '-'))
                        {
                            g.pacman.dir = down;
                            g.pacman.pos.y++;
                        }
                        else
                        {
                            if((g.pacman.dir == left) && (g.lab[py][px - 1] != '#'))
                                g.pacman.pos.x--;
                            if((g.pacman.dir == right) && (g.lab[py][px + 1] != '#'))
                                g.pacman.pos.x++;
                            if((g.pacman.dir == up) && (g.lab[py - 1][px] != '#'))
                                g.pacman.pos.y--;
                        }
                    }

    /*tp*/
    if(g.pacman.pos.x < 0) /*teleporte do pacman da esquerda*/
        g.pacman.pos.x = 19;
    if( g.pacman.pos.x > 19) /* teleporte do pacman da direita */
        g.pacman.pos.x= 0;

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief calls the ghosts moving functions
 * @responsible function by calling all the moving functions of all ghosts and killing pacman and ghosts
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

t_game fant(t_game g)
{ 
    int i;


    for(i=blinky; i <= clyde; i++)
    {
        if(g.ghost[i].mode==scatter)
        {
            g.ghost[i].tFim = time(NULL); 
            g.ghost[i].tDecorrido=difftime(g.ghost[i].tFim, g.ghost[i].tInicio);
            if(g.pacman.fase>=5)
            {
                if(g.ghost[i].tDecorrido>5)
                {
                    g.ghost[i].mode=chase;
                    g.ghost[i].tInicio=time(NULL);
                }
            }
            else
                if(g.ghost[i].tDecorrido>7)
                {
                    g.ghost[i].mode=chase;
                    g.ghost[i].tInicio=time(NULL);
                }
        }
        if(g.ghost[i].mode==chase)
        {
            g.ghost[i].tFim = time(NULL); 
            g.ghost[i].tDecorrido=difftime(g.ghost[i].tFim, g.ghost[i].tInicio);
            if(g.ghost[i].tDecorrido>20)
            {
                g.ghost[i].mode=scatter;
                g.ghost[i].tInicio=time(NULL);
            }
        }

    }

    for(i=blinky;i<=clyde;i++)
    {
        if(g.ghost[i].mode==afraid)
        {
            if(g.pacman.pos.x==g.ghost[i].pos.x+1 && g.pacman.dir==left && g.ghost[i].dir==right && g.pacman.pos.y==g.ghost[i].pos.y)
                g.ghost[i].mode=dead;
            if(g.pacman.pos.x==g.ghost[i].pos.x-1 && g.pacman.dir==right && g.ghost[i].dir==left && g.pacman.pos.y==g.ghost[i].pos.y)
                g.ghost[i].mode=dead;
            if(g.pacman.pos.y==g.ghost[i].pos.y+1 && g.pacman.dir==up && g.ghost[i].dir==down && g.pacman.pos.x==g.ghost[i].pos.x)
                g.ghost[i].mode=dead;
            if(g.pacman.pos.y==g.ghost[i].pos.y-1 && g.pacman.dir==down && g.ghost[i].dir==up && g.pacman.pos.x==g.ghost[i].pos.x)
                g.ghost[i].mode=dead;
            if(g.pacman.pos.x == g.ghost[i].pos.x && g.pacman.pos.y == g.ghost[i].pos.y)
                g.ghost[i].mode=dead;
            if(g.ghost[i].mode==dead)
                g.pacman.score+=200;
        } 
    }
    for(i=blinky;i<=clyde;i++)
        if(g.ghost[i].mode!=afraid && g.ghost[i].mode!=dead)
        {
            if(g.pacman.pos.x==g.ghost[i].pos.x+1 && g.pacman.dir==left && g.ghost[i].dir==right && g.pacman.pos.y==g.ghost[i].pos.y)
            {
                g.pacman.life--;
                g=p_init(g);
            }
            if(g.pacman.pos.x==g.ghost[i].pos.x-1 && g.pacman.dir==right && g.ghost[i].dir==left && g.pacman.pos.y==g.ghost[i].pos.y)
            {
                g.pacman.life--;
                g=p_init(g);
            }
            if(g.pacman.pos.y==g.ghost[i].pos.y+1 && g.pacman.dir==up && g.ghost[i].dir==down && g.pacman.pos.x==g.ghost[i].pos.x)
            {
                g.pacman.life--;
                g=p_init(g);
            }
            if(g.pacman.pos.y==g.ghost[i].pos.y-1 && g.pacman.dir==down && g.ghost[i].dir==up && g.pacman.pos.x==g.ghost[i].pos.x)
            {
                g.pacman.life--;
                g=p_init(g);
            }
            if(g.pacman.pos.x == g.ghost[i].pos.x && g.pacman.pos.y == g.ghost[i].pos.y)
            {
                g.pacman.life--;
                g=p_init(g);
            }
        }
    g=andablinky(g); 
    g=andainky(g);     
    g=andapinky(g);
    g=andaclyde(g);

    return g;
} 

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief blinky drive
 * @function responsible for all the mechanics behind the blinky ghost drive
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

t_game andablinky(t_game g)
{
    int pacx=g.pacman.pos.x; /* posicao do pacman em x */
    int pacy=g.pacman.pos.y; /* posicao do pacman em y */
    int bx=g.ghost[blinky].pos.x; /* bx vai receber a coordenada do blinky em x */
    int by=g.ghost[blinky].pos.y; /* by vai receber a coordenada do blinky em y */
    int dx =0; /* distancia do x favorito para o x onde ele se encontra */
    int dy =0; /* distancia do y favorito para o y onde ele se encontra */


    /*   ---------------------------------------------------------------------------- */


    if(g.ghost[blinky].mode==afraid)
        if(g.pacman.cont%2==0)
            return g;
    if(by==10 && (bx==19 || bx==18 || bx==17 || bx==16 || bx==0 || bx==1 || bx==2 || bx==3))
        if(g.pacman.cont%2==0)
            return g;

    if(g.ghost[blinky].mode ==scatter)
    {
        if((g.lab[by][bx+1]!='#' && g.ghost[blinky].dir!=left && bx!=15) || (by==1 && g.lab[by][bx+1]!='#' ))
            g.ghost[blinky].dir=right;
        else
        {
            if(g.lab[by][bx-1]!='#' && g.ghost[blinky].dir!=right && by!=9 && bx!=4 && bx!=15)
                g.ghost[blinky].dir=left;
            else
            {
                if(g.lab[by-1][bx]!='#' && g.ghost[blinky].dir!=down)
                    g.ghost[blinky].dir=up;
                else
                {
                    if(g.lab[by+1][bx]!='#' && g.ghost[blinky].dir!=up)
                        g.ghost[blinky].dir=down;
                }
            }
        }
    }

    if(g.ghost[blinky].mode == chase || g.ghost[blinky].mode==afraid || g.ghost[blinky].mode==dead)
    {
        if(g.ghost[blinky].mode==chase)
        {
            dx= bx - pacx; /* distancia do x do pacman */
            dy= by - pacy; /* distancia do y do pacman */
        }
        if(g.ghost[blinky].mode==afraid)
        {
            dx=rand()%21;
            dy=rand()%23;
        }
        if(g.ghost[blinky].mode==dead)
        {
            bx=9;
            by=7;
            if(bx == 9 && by == 7)
                g.ghost[blinky].mode = scatter;
        }

        /*------------------------------------------*/



        if(abs(dx)>=abs(dy))
        {
            if(dx>0)
            {
                if(g.lab[by][bx-1]!='#' && g.lab[by][bx-1]!='-' && g.ghost[blinky].dir!=right)
                    g.ghost[blinky].dir=left;
                else
                {
                    if(dx>0)
                    {
                        if(g.lab[by-1][bx]!='#' && g.lab[by-1][bx]!='-' && g.ghost[blinky].dir!=down)
                            g.ghost[blinky].dir=up;
                        else
                        {
                            if(g.lab[by+1][bx]!='#' && g.lab[by+1][bx]!='-' && g.ghost[blinky].dir!=up)
                                g.ghost[blinky].dir=down;
                            else
                                if(g.lab[by][bx+1]!='#' && g.ghost[blinky].dir!=left)
                                    g.ghost[blinky].dir=right;
                        }
                    }
                    else
                    {
                        if(g.lab[by+1][bx]!='#' && g.lab[by+1][bx]!='-' && g.ghost[blinky].dir!=up)
                            g.ghost[blinky].dir=down;
                        else
                        {
                            if(g.lab[by-1][bx]!='#' && g.lab[by-1][bx]!='-' && g.ghost[blinky].dir!=down)
                                g.ghost[blinky].dir=up;
                            else
                                if(g.lab[by][bx+1]!='#' && g.ghost[blinky].dir!=left)
                                    g.ghost[blinky].dir=right;
                        }
                    }
                }

            }
            else
            {
                if(g.lab[by][bx+1] != '#' && g.ghost[blinky].dir!=left)
                    g.ghost[blinky].dir=right;
                else
                    if(dy>0)
                    {
                        if(g.lab[by-1][bx] != '#' && g.ghost[blinky].dir!=down && g.lab[by-1][bx]!='-')
                            g.ghost[blinky].dir=up;
                        else
                        {
                            if(g.lab[by+1][bx]!='#' && g.ghost[blinky].dir!=up && g.lab[by+1][bx]!='-')
                                g.ghost[blinky].dir=down;
                            else
                                if(g.lab[by][bx-1]!='#' && g.ghost[blinky].dir!=right)
                                    g.ghost[blinky].dir=left;
                        }
                    }   
                    else
                    {
                        if(g.lab[by+1][bx] != '#' && g.ghost[blinky].dir!=up && g.lab[by+1][bx]!='-')
                            g.ghost[blinky].dir=down;
                        else
                        {
                            if(g.lab[by-1][bx]!='#' && g.ghost[blinky].dir!=down && g.lab[by-1][bx]!='-')
                                g.ghost[blinky].dir=up;
                            else
                                if(g.lab[by][bx-1]!='#' && g.ghost[blinky].dir!=right)
                                    g.ghost[blinky].dir=left;
                        }
                    }
            }

        }
        else
        {
            if(dy>0)
            {
                if(g.lab[by-1][bx] != '#' && g.ghost[blinky].dir!=down && g.lab[by-1][bx]!='-')
                {
                    g.ghost[blinky].dir=up;
                    g.ghost[blinky].pos.y--;
                }
                else
                {
                    if(dx>0)
                    {
                        if(g.lab[by][bx-1] != '#' && g.ghost[blinky].dir!=right)
                        {
                            g.ghost[blinky].dir=left;
                            g.ghost[blinky].pos.x--;
                        }
                        else
                        {
                            if(g.lab[by][bx+1]!='#' && g.ghost[blinky].dir!=left)
                            {
                                g.ghost[blinky].dir=right;
                                g.ghost[blinky].pos.x++;
                            }
                            else
                                if(g.lab[by+1][bx]!='#' && g.ghost[blinky].dir!=up && g.lab[by+1][bx]!='-')
                                {
                                    g.ghost[blinky].dir=down;
                                    g.ghost[blinky].pos.y++;
                                }
                        }
                    }
                    else
                    {
                        if(g.lab[by][bx+1] != '#' && g.ghost[blinky].dir!=left)
                        {
                            g.ghost[blinky].dir=right;
                            g.ghost[blinky].pos.x++;
                        }
                        else
                        {
                            if(g.lab[by][bx-1]!='#' && g.ghost[blinky].dir!=right)
                            {
                                g.ghost[blinky].dir=left;
                                g.ghost[blinky].pos.x--;
                            }
                            else
                                if(g.lab[by+1][bx]!='#' && g.ghost[blinky].dir!=up && g.lab[by+1][bx]!='-')
                                {
                                    g.ghost[blinky].dir=down;
                                    g.ghost[blinky].pos.y++;
                                }
                        }
                    }
                }
            }
            else
            {
                if(g.lab[by+1][bx] != '#' && g.ghost[blinky].dir!=up && g.lab[by+1][bx]!='-')
                {
                    g.ghost[blinky].dir=down;
                    g.ghost[blinky].pos.y++;
                }
                else
                {
                    if(dx>0)
                    {
                        if(g.lab[by][bx-1] != '#' && g.ghost[blinky].dir!=right)
                        {
                            g.ghost[blinky].dir=left;
                            g.ghost[blinky].pos.x--;
                        }
                        else
                        {
                            if(g.lab[by][bx+1]!='#' && g.ghost[blinky].dir!=left)
                            {
                                g.ghost[blinky].dir=right;
                                g.ghost[blinky].pos.x++;
                            }
                            else
                                if(g.lab[by-1][bx]!='#' && g.ghost[blinky].dir!=down && g.lab[by-1][bx]!='-')
                                {
                                    g.ghost[blinky].dir=up;
                                    g.ghost[blinky].pos.y--;
                                }
                        }
                    }
                    else
                    {
                        if(g.lab[by][bx+1] != '#' && g.ghost[blinky].dir!=left)
                        {
                            g.ghost[blinky].dir=right;
                            g.ghost[blinky].pos.x++;
                        }
                        else
                        {
                            if(g.lab[by][bx-1]!='#' && g.ghost[blinky].dir!=right)
                            {
                                g.ghost[blinky].dir=left;
                                g.ghost[blinky].pos.x--;
                            }
                            else
                                if(g.lab[by-1][bx]!='#' && g.ghost[blinky].dir!=down && g.lab[by-1][bx]!='-')
                                {
                                    g.ghost[blinky].dir=up;
                                    g.ghost[blinky].pos.y--;
                                }
                        }
                    }
                }
            }
        }
    }


    /* caso o fantasma nao esteja no modo afraid os targets voltam ao normal */

    if(g.ghost[blinky].dir==right)
        bx++;
    if(g.ghost[blinky].dir==left)
        bx--;
    if(g.ghost[blinky].dir==up)
        by--;
    if(g.ghost[blinky].dir==down)
        by++;

    /*----------------------------------------------------------*/

    /* teleporte blinky esquerda */

    if(g.ghost[blinky].pos.x <0) 
        g.ghost[blinky].pos.x =19;

    /* teleporte blinky direita */

    if(g.ghost[blinky].pos.x > 19) 
        g.ghost[blinky].pos.x = 0;


    g.ghost[blinky].pos.x=bx;
    g.ghost[blinky].pos.y=by;

    return g;
}

/* ---------------------------------------------------------------------- */

/** 
 * @ingroup upe-blink
 * @brief inky drive
 * @function responsible for all the mechanics behind the inky ghost drive
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 * */

t_game andainky(t_game g)
{
    int pacx = g.pacman.pos.x; /* posicao do pacman em x */
    int pacy = g.pacman.pos.y;   /*posicao do pacman em y*/
    int ix = g.ghost[inky].pos.x;  /* ix vai receber a coordenada do inky em x */
    int iy = g.ghost[inky].pos.y;   /*iy vai receber a coordenada do inky em y */
    int bix = g.ghost[blinky].pos.x;  /* bx vai receber a coordenada do blinky em x */
    int biy = g.ghost[blinky].pos.y;  /* by vai receber a coordenada do blinky em y */
    int ity = g.ghost[inky].starget.y;  /* preferred corner (line) */
    int itx = g.ghost[inky].starget.x;  /* preferred corner (column) */
    int distx,disty;
    char hif;

    if(g.ghost[inky].mode==afraid)
        if(g.pacman.cont%2==0)
            return g;
    if(iy==10 && (ix==19 || ix==18 || ix==17 || ix==16 || ix==0 || ix==1 || ix==2 || ix==3))
        if(g.pacman.cont%2==0)
            return g;
    if(g.pacman.dots<15)
        return g;
    srand(time(NULL)); /*set seed rand*/
    if(g.ghost[inky].mode==scatter)
    {
        ity = g.ghost[inky].starget.y;
        itx = g.ghost[inky].starget.x;
    }
    if(g.ghost[inky].mode==afraid)
    {
        if(g.pacman.revert==0)
        {
            if(g.ghost[inky].dir==left)
                g.ghost[inky].dir=right;
            if(g.ghost[inky].dir==right)
                g.ghost[inky].dir=left;
            if(g.ghost[inky].dir==up)
                g.ghost[inky].dir=down;
            if(g.ghost[inky].dir==down)
                g.ghost[inky].dir=up;
        }

        /*rand para escolha de lado em bifurcacao fantasma*/
        itx = rand()%21;
        ity = rand()%23;
    }
    if(g.ghost[inky].mode==dead)
    {
        hif='!';
        itx=9;
        ity=9;
        if(iy==9 && ix==9)
            g.ghost[inky].mode=scatter;
    }
    else
    {
        if((ix == 8 || ix == 9 || ix ==10 || ix==11) && (iy == 8 || iy == 9 || iy ==10 || iy == 11))
            hif='!';
        else
            hif='-';
    }
    if(g.ghost[inky].mode==chase)
    {
        if(pacx-ix<7 && pacy-iy<7)
        {
            if(abs(bix-pacx)>=abs(biy-pacy))
            {
                if(bix-pacx<0)
                {
                    itx=pacx+1;
                    ity=pacy;
                }
                else
                {
                    itx=pacx-1;
                    ity=pacy;
                }
            }
            else
            {
                if(biy-pacy<0)
                {
                    itx=pacx;
                    ity=pacy+1;
                }
                else
                {
                    itx=pacx;
                    ity=pacy-1;
                }
            }
        }
        else
        { 
            itx = rand()%21;
            ity = rand()%23;
        }
    }
    if(g.ghost[inky].mode!=dead)
        if((ix == 8 || ix == 9 || ix ==10 || ix==11) && (iy == 8 || iy == 9 || iy ==10 || iy == 11))
        {
            itx=10;
            ity=6;
        } 
    distx=ix-itx;
    disty=iy-ity;
    if(abs(distx)>=abs(disty))
    {
        if(distx>0)
        {
            if(g.lab[iy][ix-1] != '#' && g.ghost[inky].dir!=right)
                g.ghost[inky].dir=left;
            else
            {
                if(disty>0)
                {
                    if(g.lab[iy-1][ix] != '#' && g.ghost[inky].dir!=down && g.lab[iy-1][ix]!=hif)
                        g.ghost[inky].dir=up;
                    else
                    {
                        if(g.lab[iy+1][ix]!='#' && g.ghost[inky].dir!=up && g.lab[iy+1][ix]!=hif)
                            g.ghost[inky].dir=down;
                        else
                            g.ghost[inky].dir=right;
                    }
                }
                else
                {
                    if(g.lab[iy+1][ix] != '#' && g.ghost[inky].dir!=up && g.lab[iy+1][ix]!=hif)
                        g.ghost[inky].dir=down;
                    else
                    {
                        if(g.lab[iy-1][ix]!='#' && g.ghost[inky].dir!=down && g.lab[iy-1][ix]!=hif)
                            g.ghost[inky].dir=up;
                        else
                            g.ghost[inky].dir=right;
                    }
                }
            }
        }
        else
        {
            if(g.lab[iy][ix+1] != '#' && g.ghost[inky].dir!=left)
                g.ghost[inky].dir=right;
            else
            {
                if(disty>0)
                {
                    if(g.lab[iy-1][ix] != '#' && g.ghost[inky].dir!=down && g.lab[iy-1][ix]!=hif)
                        g.ghost[inky].dir=up;
                    else
                    {
                        if(g.lab[iy+1][ix]!='#' && g.ghost[inky].dir!=up && g.lab[iy+1][ix]!=hif)
                            g.ghost[inky].dir=down;
                        else
                            g.ghost[inky].dir=left;
                    }
                }
                else
                {
                    if(g.lab[iy+1][ix] != '#' && g.ghost[inky].dir!=up && g.lab[iy+1][ix]!=hif)
                        g.ghost[inky].dir=down;
                    else
                    {
                        if(g.lab[iy-1][ix]!='#' && g.ghost[inky].dir!=down && g.lab[iy-1][ix]!=hif)
                            g.ghost[inky].dir=up;
                        else
                            g.ghost[inky].dir=left;
                    }
                }
            }
        }
    }
    else
    {
        if(disty>0)
        {
            if(g.lab[iy-1][ix] != '#' && g.ghost[inky].dir!=down && g.lab[iy-1][ix]!=hif)
                g.ghost[inky].dir=up;
            else
            {
                if(distx>0)
                {
                    if(g.lab[iy][ix-1] != '#' && g.ghost[inky].dir!=right)
                        g.ghost[inky].dir=left;
                    else
                    {
                        if(g.lab[iy][ix+1]!='#' && g.ghost[inky].dir!=left)
                            g.ghost[inky].dir=right;
                        else
                            g.ghost[inky].dir=down;
                    }
                }
                else
                {
                    if(g.lab[iy][ix+1] != '#' && g.ghost[inky].dir!=left)
                        g.ghost[inky].dir=right;
                    else
                    {
                        if(g.lab[iy][ix-1]!='#' && g.ghost[inky].dir!=right)
                            g.ghost[inky].dir=left;
                        else
                            g.ghost[inky].dir=down;
                    }
                }
            }
        }
        else
        {
            if(g.lab[iy+1][ix] != '#' && g.ghost[inky].dir!=up && g.lab[iy+1][ix]!=hif)
                g.ghost[inky].dir=down;
            else
            {
                if(distx>0)
                {
                    if(g.lab[iy][ix-1] != '#' && g.ghost[inky].dir!=right)
                        g.ghost[inky].dir=left;
                    else
                        if(g.lab[iy][ix+1]!='#' && g.ghost[inky].dir!=left)
                            g.ghost[inky].dir=right;
                        else
                            g.ghost[inky].dir=up;
                }
                else
                {
                    if(g.lab[iy][ix+1] != '#' && g.ghost[inky].dir!=left)
                        g.ghost[inky].dir=right;
                    else
                    {
                        if(g.lab[iy][ix-1]!='#' && g.ghost[inky].dir!=right)
                            g.ghost[inky].dir=left;
                        else
                            g.ghost[inky].dir=up;
                    }
                }
            }
        }
    }

    if(g.ghost[inky].dir==up)
        g.ghost[inky].pos.y--;
    if(g.ghost[inky].dir==down)
        g.ghost[inky].pos.y++;
    if(g.ghost[inky].dir==left)
        g.ghost[inky].pos.x--;
    if(g.ghost[inky].dir==right)
        g.ghost[inky].pos.x++;

    if(g.ghost[inky].pos.x < 0)
        g.ghost[inky].pos.x = 19;
    if(g.ghost[inky].pos.x > 19)
        g.ghost[inky].pos.x = 0;

    return g;
} 

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief pinky drive
 * @function responsible for all the mechanics behind the pinky ghost drive
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */ 

t_game andapinky(t_game g)
{
    int pacx = g.pacman.pos.x;     /* posicao do pacman em x */
    int pacy = g.pacman.pos.y;     /* posicao do pacman em y */ 
    int gx = g.ghost[pinky].pos.x; /* posicao do fantasma em x */
    int gy = g.ghost[pinky].pos.y; /* posicao do fantasma em y */
    int targetx = 0;                 /* target do fantasma em x */
    int targety = 0;                 /* target do fantasma em y */ 
    int tx=g.ghost[pinky].starget.x; /*Canto x preferido do pinky no modo scatter  */
    int ty=g.ghost[pinky].starget.y; /* Canto y preferido do pinky no modo scatter */
    int nrand;                       /* Variavel para gerar o aleatorio do afraid  */

    if(g.ghost[pinky].mode==afraid)
        if(g.pacman.cont%2==0)
            return g;
    if(gy==10 && (gx==19 || gx==18 || gx==17 || gx==16 || gx==0 || gx==1 || gx==2 || gx==3))
        if(g.pacman.cont%2==0)
            return g;
    if(g.pacman.dir==right)
    {
        targetx = pacx + 4;
        targety = pacy;
    }
    if(g.pacman.dir==left)
    {
        targetx = pacx - 4;
        targety = pacy;
    }
    if(g.pacman.dir==up)
    {
        targety = pacy - 4;
        targetx = pacx;
    }
    if(g.pacman.dir==down)
    {
        targety = pacy + 4;
        targetx = pacx;
    }

    if(g.pacman.dots<6)
        return g;

    if(g.ghost[pinky].mode==chase)
    {
        if(gx==10 && gy==9)
            g.ghost[pinky].dir=up;
        if(g.ghost[pinky].dir==up && g.lab[gy-1][gx]=='#')
        {
            if(targetx>=targety)
            {
                if(g.lab[gy][gx+1]!='#')
                    g.ghost[pinky].dir=right;
                else
                    g.ghost[pinky].dir=left;
            }
            else
                g.ghost[pinky].dir=down;
        }
        else if(g.ghost[pinky].dir==up && (g.lab[gy][gx+1]!='#' && g.lab[gy][gx-1]!='#'))
        {
            if(targetx>gx && targetx<targety && g.lab[gy][gx+1]!='#')
                g.ghost[pinky].dir=right;
            if(targetx<gx && targetx<targety && g.lab[gy][gx-1]!='#')
                g.ghost[pinky].dir=left;
        }

        if(g.ghost[pinky].dir==down && (g.lab[gy+1][gx]=='#' || g.lab[gy+1][gx]=='-'))
        {
            if(targetx>targety)
            {
                if(targetx>gx && g.lab[gy][gx+1]!='#')
                    g.ghost[pinky].dir=right;
                else
                    g.ghost[pinky].dir=left;
            }
            else
                g.ghost[pinky].dir=up;
        }
        else if(g.ghost[pinky].dir==down && (g.lab[gy][gx+1]!='#' || g.lab[gy][gx-1]!='#'))
        {
            if(targetx>gx && targetx<targety && g.lab[gy][gx+1]!='#')
                g.ghost[pinky].dir=right;
            if(targetx<gx && targetx<targety && g.lab[gy][gx-1]!='#')
                g.ghost[pinky].dir=left;
        }
        if(g.ghost[pinky].dir==left && g.lab[gy][gx-1]=='#')
        {
            if(targetx>=targety)
            {
                if(targety>gy && g.lab[gy+1][gx]!='#')
                    g.ghost[pinky].dir=down;
                else
                    g.ghost[pinky].dir=up;
            }
            else
                g.ghost[pinky].dir=right;
        }
        else if(g.ghost[pinky].dir==left && (g.lab[gy+1][gx]!='#' || g.lab[gy-1][gx]!='#'))
        {
            if(targety>gy && targetx>targety && g.lab[gy+1][gx]!='#')
                g.ghost[pinky].dir=down;
            if(targety<gy && targetx>targety && g.lab[gy-1][gx]!='#')
                g.ghost[pinky].dir=up;
        }

        if(g.ghost[pinky].dir==right && g.lab[gy][gx+1]=='#')
        {
            if(targetx<targety)
            {
                if(targety>gy && g.lab[gy+1][gx]!='#')
                    g.ghost[pinky].dir=down;
                else
                    g.ghost[pinky].dir=up;
            }
            else
                g.ghost[pinky].dir=left;
        }
        else if(g.ghost[pinky].dir==right && (g.lab[gy+1][gx]!='#' || g.lab[gy-1][gx]!='#'))
        {
            if(targety>gy && targetx<targety && g.lab[gy+1][gx]!='#')
                g.ghost[pinky].dir=down;
            if(targety<gy && targetx<targety && g.lab[gy-1][gx]!='#')
                g.ghost[pinky].dir=up;
        }
        if(g.ghost[pinky].dir==right)
            gx++;
        if(g.ghost[pinky].dir==left)
            gx--;
        if(g.ghost[pinky].dir==up)
            gy--;
        if(g.ghost[pinky].dir==down)
            gy++;
    }
    if(g.ghost[pinky].mode==afraid)
    {
        srand(time(NULL));
        nrand=rand();

        /*Primeiro rand do afraid*/
        if(nrand%4==0)
        {
            if(g.lab[gy-1][gx] !='#' && g.lab[gy-1][gx] !='-')
            {
                g.ghost[pinky].dir=up;
                gy--;
            }
            else
            {
                if(g.lab[gy][gx+1] !='#' && g.lab[gy][gx+1] !='-')
                {
                    g.ghost[pinky].dir=right;
                    gx++;
                }
                else
                {
                    if(g.lab[gy][gx-1] !='#' && g.lab[gy][gx-1] !='-')
                    {
                        g.ghost[pinky].dir=left;
                        gx--;
                    }
                    else
                        if(g.lab[gy+1][gx] !='#' && g.lab[gy+1][gx] !='-')
                        {
                            g.ghost[pinky].dir=down;
                            gy++;
                        }
                }
            }
        }
        /*Segundo rand afraid*/
        else
        {
            if(nrand%4==1)
            {
                if(g.lab[gy][gx-1] !='#' && g.lab[gy][gx-1] !='-')
                {
                    g.ghost[pinky].dir=left;
                    gx--;
                }
                else
                {
                    if(g.lab[gy][gx+1] !='#' && g.lab[gy][gx+1] !='-')
                    {
                        g.ghost[pinky].dir=right;
                        gx++;
                    }
                    else
                    {
                        if(g.lab[gy-1][gx] !='#' && g.lab[gy-1][gx] !='-')
                        {
                            g.ghost[pinky].dir=up;
                            gy--;
                        }
                        else
                            if(g.lab[gy+1][gx] !='#' && g.lab[gy+1][gx] !='-')
                            {
                                g.ghost[pinky].dir=down;
                                gy++;
                            }
                    }
                }
            }
            /*Terceiro rand afraid*/
            else
            {
                if(nrand%4==2)
                {
                    if(g.lab[gy+1][gx] !='#' && g.lab[gy+1][gx] !='-')
                    {
                        g.ghost[pinky].dir=down;
                        gy++;
                    }
                    else
                    {
                        if(g.lab[gy][gx+1] !='#' && g.lab[gy][gx+1] !='-')
                        {
                            g.ghost[pinky].dir=right;
                            gx++;
                        }
                        else
                        {
                            if(g.lab[gy-1][gx] !='#' && g.lab[gy][gy+1] !='-')
                            {
                                g.ghost[pinky].dir=up;
                                gy--;
                            }
                            else
                                if(g.lab[gy][gx-1] !='#' && g.lab[gy][gx-1] !='-')
                                {
                                    g.ghost[pinky].dir=left;
                                    gx--;
                                }
                        }
                    }
                }
                /*Quarto rand afraid*/
                else
                {
                    if(nrand%4==3)
                    {
                        if(g.lab[gy][gx+1] !='#' && g.lab[gy][gx+1] !='-')
                        {
                            g.ghost[pinky].dir=right;
                            gx++;
                        }
                        else
                        {
                            if(g.lab[gy-1][gx] !='#' && g.lab[gy-1][gx] !='-')
                            {
                                g.ghost[pinky].dir=up;
                                gy--;
                            }
                            else
                            {
                                if(g.lab[gy][gx-1] !='#' && g.lab[gy][gx-1] !='-')
                                {
                                    g.ghost[pinky].dir=left;
                                    gx--;
                                }
                                else
                                    if(g.lab[gy+1][gx] !='#' && g.lab[gy+1][gx]!='-')
                                    {
                                        g.ghost[pinky].dir=down;
                                        gy++;
                                    }
                            }
                        }
                    }
                }
            }
        }
    }

    if(g.ghost[pinky].mode==scatter)
    {
        /*linhas e colunas probidadas, debug da casa*/
        if((gx == 9 || gx == 10 || gx == 11) && (gy == 9 || gy == 10 || gy == 11))
        {
            ty=1;
            tx=1;
        }
        gy=gy-ty;
        gx=gx-tx;

        /*Rota do canto preferido do pinky*/
        if((g.lab[gy][gx-1]!='#' && g.ghost[pinky].dir !=right && gx!=4) || (g.lab[gy][gx-1]!='#' && gy==1))
        {
            g.ghost[pinky].dir=left;
            gx--;
        }
        else
        {
            if(g.lab[gy-1][gx]!='#' && g.ghost[pinky].dir!=down)
            {
                g.ghost[pinky].dir=up;
                gy--;
            }
            else
            {
                if(g.lab[gy][gx+1]!='#' && g.ghost[pinky].dir!=left)
                {
                    g.ghost[pinky].dir=right;
                    gx++;
                }
                else
                    if(g.lab[gy+1][gx]!='#' && g.ghost[pinky].dir!=up)
                    {
                        g.ghost[pinky].dir=down;
                        gy++;
                    }
            }
        }
    }

    if(g.ghost[pinky].mode==dead)
    {
        gx=10;
        gy=9;
        if(gx==10&&gy==9)
            g.ghost[pinky].mode=scatter;
        g.ghost[pinky].dir=up;
    }

    /* Teleporte do tunel */
    if(g.ghost[pinky].pos.x <= 0)
        gx=19;
    if(g.ghost[pinky].pos.x >= 19)
        gx=0;
    g.ghost[pinky].pos.x = gx;
    g.ghost[pinky].pos.y = gy;

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief clyde drive
 * @function responsible for all the mechanics behind the clyde ghost drive
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

t_game andaclyde(t_game g)
{
    int gx = g.ghost[clyde].pos.x,                /*recebe a cordenada x do clyde*/ 
        gy = g.ghost[clyde].pos.y,               /*recebe a cordenada y do clyde*/
        dir = g.ghost[clyde].dir,
        mode = g.ghost[clyde].mode,           /*recebe o modo em que o pacman esta*/
        numr;

    double dist[4],
    distpacman,
    menordist=MAIORDIST;

    t_pos alvo;

    int i, auxdir;

    srand(time(NULL));
    numr= rand();

    distpacman=distpontos(gy,gx,g.pacman.pos);

    if(g.pacman.dots<20) /*retira o clyde da casa apenas depois do pacman comer 20 pallets*/
        return g;

    switch(mode)
    {
        case dead : alvo.x = 10;
                    alvo.y = 11;

                    if(gx == 10 && gy == 10)
                    {
                        g.ghost[clyde].mode=scatter;
                        return g;
                    }

                    if(gx == 10 && gy <= 11 && gy >=8)
                    {
                        --g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=up;
                        return g;
                    }

                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#')
                    {
                        ++g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=down;

                        return g;
                    }

                    if(g.lab[gy][gx-1] == '#' && g.lab[gy+1][gx] == '#' && g.lab[gy-1][gx] == '#')
                    {
                        ++g.ghost[clyde].pos.x;
                        g.ghost[clyde].dir=right;
                        return g;
                    } 

                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy+1][gx] == '#')
                    {
                        --g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=up;
                        return g;
                    } 

                    if(g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] == '#')
                    {
                        --g.ghost[clyde].pos.x;
                        g.ghost[clyde].dir=left;
                        return g;
                    } 


                    if(g.lab[gy-1][gx] != '#' && dir != down)
                        dist[up]=distpontos(gy-1, gx, alvo);
                    else
                        dist[up]=MAIORDIST;

                    if(g.lab[gy][gx-1] != '#' && dir != right)
                        dist[left]=distpontos(gy, gx-1, alvo);
                    else 
                        dist[left]=MAIORDIST;

                    if(g.lab[gy+1][gx] != '#' && dir != up)
                        dist[down]=distpontos(gy+1, gx, alvo);
                    else
                        dist[down]=MAIORDIST;

                    if(g.lab[gy][gx+1] != '#' && dir != left)
                        dist[right]=distpontos(gy, gx+1, alvo);
                    else
                        dist[right]=MAIORDIST;

                    for(i=0; i<4; i++)
                        if(i==0)
                        {
                            menordist=dist[i];
                            auxdir=i;
                        }
                        else
                            if(dist[i] <= menordist)
                            {
                                menordist=dist[i];
                                auxdir=i;
                            }

                    switch(auxdir)
                    {
                        case up :   --g.ghost[clyde].pos.y;
                                    g.ghost[clyde].dir=up;
                                    break;

                        case left :  --g.ghost[clyde].pos.x;
                                     g.ghost[clyde].dir=left;
                                     break;

                        case down :  ++g.ghost[clyde].pos.y;
                                     g.ghost[clyde].dir=down;
                                     break;

                        case right :  ++g.ghost[clyde].pos.x;
                                      g.ghost[clyde].dir=right;
                                      break;
                    }



                    return g;


        case afraid :

                    if(g.pacman.cont % 2 == 0)
                        return g;

                    if(numr %  4 == 0)
                    {
                        if(g.lab[gy - 1][gx] != '#' && dir != down)
                        {
                            g.ghost[clyde].pos.y--;
                            g.ghost[clyde].dir=up;
                        }
                        else
                            if(g.lab[gy][gx + 1] != '#' && dir != left)
                            {
                                g.ghost[clyde].pos.x++;
                                g.ghost[clyde].dir=right;
                            }
                            else
                                if(g.lab[gy][gx - 1] != '#' && dir != right)
                                {
                                    g.ghost[clyde].pos.x--;
                                    g.ghost[clyde].dir=left;
                                }
                                else
                                    if(g.lab[gy + 1][gx] != '#' && g.lab[gy + 1][gx] != '-' && dir != up)
                                    {
                                        g.ghost[clyde].pos.y++;
                                        g.ghost[clyde].dir=down;
                                    }

                    }
                    else
                        if(numr % 4 == 1)
                        {
                            if(g.lab[gy][gx - 1] != '#' && dir != right)
                            {
                                g.ghost[clyde].pos.x--;
                                g.ghost[clyde].dir=left;
                            }
                            else
                                if(g.lab[gy][gx + 1] != '#' && dir != left)
                                {
                                    g.ghost[clyde].pos.x++;
                                    g.ghost[clyde].dir=right;
                                }
                                else
                                    if(g.lab[gy - 1][gx] != '#' && dir != down)
                                    {
                                        g.ghost[clyde].pos.y--;
                                        g.ghost[clyde].dir=up;
                                    }
                                    else
                                        if(g.lab[gy + 1][gx] != '#' && g.lab[gy + 1][gx] != '-' && dir != up)
                                        {
                                            g.ghost[clyde].pos.y++;
                                            g.ghost[clyde].dir=down;
                                        }
                        } 
                        else
                            if(numr % 4 == 2) 
                            { 
                                if(g.lab[gy + 1][gx] != '#' && g.lab[gy + 1][gx] != '-' && dir != up)
                                {
                                    g.ghost[clyde].pos.y++;
                                    g.ghost[clyde].dir=down;
                                }
                                else
                                    if(g.lab[gy][gx + 1] != '#' && dir != left)
                                    {
                                        g.ghost[clyde].pos.x++;
                                        g.ghost[clyde].dir=right;
                                    }
                                    else
                                        if(g.lab[gy - 1][gx] != '#' && dir != down)
                                        {
                                            g.ghost[clyde].pos.y--;
                                            g.ghost[clyde].dir=up;
                                        }
                                        else
                                            if(g.lab[gy][gx - 1] != '#' && dir != right)
                                            {
                                                g.ghost[clyde].pos.x--;
                                                g.ghost[clyde].dir=left;
                                            }
                            }
                            else
                                if(numr % 4 == 3)
                                { 
                                    if(g.lab[gy][gx + 1] != '#' && dir != left)
                                    {
                                        g.ghost[clyde].pos.x++;
                                        g.ghost[clyde].dir=right;
                                    }
                                    else
                                        if(g.lab[gy - 1][gx] != '#' && dir != down)
                                        {
                                            g.ghost[clyde].pos.y--;
                                            g.ghost[clyde].dir=up;
                                        }
                                        else
                                            if(g.lab[gy][gx - 1] != '#' && dir != right)
                                            {
                                                g.ghost[clyde].pos.x--;
                                                g.ghost[clyde].dir=left;
                                            }
                                            else
                                                if(g.lab[gy + 1][gx] != '#' && g.lab[gy + 1][gx] != '-' && dir != up)
                                                {
                                                    g.ghost[clyde].pos.y++;
                                                    g.ghost[clyde].dir=down;
                                                }
                                }

                    break;

        case chase : if(distpacman > 8)
                     {  
                         alvo.x = g.pacman.pos.x;
                         alvo.y = g.pacman.pos.y;

                         if(gx == 10 && gy <= 11 && gy >=8)
                         {
                             --g.ghost[clyde].pos.y;
                             g.ghost[clyde].dir=up;
                             return g;
                         }

                         if(gx == 10 && gy <= 11 && gy >=8)
                         {
                             --g.ghost[clyde].pos.y;
                             g.ghost[clyde].dir=up;
                             return g;
                         }


                         if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] != '-')
                         {
                             ++g.ghost[clyde].pos.y;
                             g.ghost[clyde].dir=down;
                             return g;
                         }

                         if(g.lab[gy][gx-1] == '#' && g.lab[gy+1][gx] == '#' && g.lab[gy-1][gx] == '#')
                         {
                             ++g.ghost[clyde].pos.x;
                             g.ghost[clyde].dir=right;
                             return g;
                         } 

                         if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy+1][gx] == '#')
                         {
                             --g.ghost[clyde].pos.y;
                             g.ghost[clyde].dir=up;
                             return g;
                         } 

                         if(g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] == '#')
                         {
                             --g.ghost[clyde].pos.x;
                             g.ghost[clyde].dir=left;
                             return g;
                         } 


                         if(g.lab[gy-1][gx] != '#' && dir != down)
                             dist[0]=distpontos(gy-1, gx, alvo);
                         else
                             dist[0]=MAIORDIST;

                         if(g.lab[gy][gx-1] != '#' && dir != right)
                             dist[1]=distpontos(gy, gx-1, alvo);
                         else 
                             dist[1]=MAIORDIST;

                         if(g.lab[gy+1][gx] != '#' && g.lab[gy+1][gx] != '-'&& dir != up)
                             dist[2]=distpontos(gy+1, gx, alvo);
                         else
                             dist[2]=MAIORDIST;

                         if(g.lab[gy][gx+1] != '#' && dir != left) 
                             dist[3]=distpontos(gy, gx+1, alvo);
                         else
                             dist[3]=MAIORDIST;

                         for(i=0; i<4; i++)
                             if(i==0)
                             {
                                 menordist=dist[i];
                                 auxdir=i;
                             }
                             else
                                 if(dist[i] <= menordist)
                                 {
                                     menordist=dist[i];
                                     auxdir=i;
                                 }

                         switch(auxdir)
                         {
                             case up :   --g.ghost[clyde].pos.y;
                                         g.ghost[clyde].dir=up;
                                         return g;

                             case left : --g.ghost[clyde].pos.x;
                                         g.ghost[clyde].dir=left;
                                         return g;

                             case down : ++g.ghost[clyde].pos.y;
                                         g.ghost[clyde].dir=down;
                                         return g;

                             case right :++g.ghost[clyde].pos.x;
                                         g.ghost[clyde].dir=right;
                                         return g;
                         }


                     }
                                else
                                {
                                    if(gx == 10 && gy <= 11 && gy >=8)
                                    {
                                        --g.ghost[clyde].pos.y;
                                        g.ghost[clyde].dir=up;
                                        return g;
                                    }

                                    alvo.x = g.ghost[clyde].starget.x;
                                    alvo.y = g.ghost[clyde].starget.y;

                                    if(gx == alvo.x && gy == alvo.y)
                                    {
                                        g.ghost[clyde].dir=right;
                                        ++g.ghost[clyde].pos.x;
                                        return g;
                                    }


                                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] != '-')
                                    {
                                        ++g.ghost[clyde].pos.y;
                                        g.ghost[clyde].dir=down;

                                        return g;
                                    }

                                    if(g.lab[gy][gx-1] == '#' && g.lab[gy+1][gx] == '#' && g.lab[gy-1][gx] == '#')
                                    {
                                        ++g.ghost[clyde].pos.x;
                                        g.ghost[clyde].dir=right;
                                        return g;
                                    } 

                                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy+1][gx] == '#')
                                    {
                                        --g.ghost[clyde].pos.y;
                                        g.ghost[clyde].dir=up;
                                        return g;
                                    } 

                                    if(g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] == '#')
                                    {
                                        --g.ghost[clyde].pos.x;
                                        g.ghost[clyde].dir=left;
                                        return g;
                                    } 


                                    if(g.lab[gy-1][gx] != '#' && dir != down)
                                        dist[up]=distpontos(gy-1, gx, alvo);
                                    else
                                        dist[up]=MAIORDIST;

                                    if(g.lab[gy][gx-1] != '#' && dir != right)
                                        dist[left]=distpontos(gy, gx-1, alvo);
                                    else 
                                        dist[left]=MAIORDIST;

                                    if(g.lab[gy+1][gx] != '#' && g.lab[gy+1][gx] != '-' && dir != up)
                                        dist[down]=distpontos(gy+1, gx, alvo);
                                    else
                                        dist[down]=MAIORDIST;

                                    if(g.lab[gy][gx+1] != '#' && dir != left)
                                        dist[right]=distpontos(gy, gx+1, alvo);
                                    else
                                        dist[right]=MAIORDIST;

                                    for(i=0; i<4; i++)
                                        if(dist[i] < menordist)
                                        {
                                            menordist=dist[i];
                                            auxdir=i;
                                        }

                                    switch(auxdir)
                                    {
                                        case up :   --g.ghost[clyde].pos.y;
                                                    g.ghost[clyde].dir=up;
                                                    break;

                                        case left :  --g.ghost[clyde].pos.x;
                                                     g.ghost[clyde].dir=left;
                                                     break;

                                        case down :  ++g.ghost[clyde].pos.y;
                                                     g.ghost[clyde].dir=down;
                                                     break;

                                        case right :  ++g.ghost[clyde].pos.x;
                                                      g.ghost[clyde].dir=right;
                                                      break;
                                    }


                                    return g;


                                }


                    break;

        case scatter :

                    if(gx == 10 && gy <= 11 && gy >=8)
                    {
                        --g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=up;
                        return g;
                    }

                    alvo.x = g.ghost[clyde].starget.x;
                    alvo.y = g.ghost[clyde].starget.y;

                    if(gx == alvo.x && gy == alvo.y)
                    {
                        g.ghost[clyde].dir=right;
                        ++g.ghost[clyde].pos.x;
                        return g;
                    }


                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] != '-')
                    {
                        ++g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=down;

                        return g;
                    }

                    if(g.lab[gy][gx-1] == '#' && g.lab[gy+1][gx] == '#' && g.lab[gy-1][gx] == '#')
                    {
                        ++g.ghost[clyde].pos.x;
                        g.ghost[clyde].dir=right;
                        return g;
                    } 

                    if(g.lab[gy][gx-1] == '#' && g.lab[gy][gx+1] == '#' && g.lab[gy+1][gx] == '#')
                    {
                        --g.ghost[clyde].pos.y;
                        g.ghost[clyde].dir=up;
                        return g;
                    } 

                    if(g.lab[gy][gx+1] == '#' && g.lab[gy-1][gx] == '#' && g.lab[gy+1][gx] == '#')
                    {
                        --g.ghost[clyde].pos.x;
                        g.ghost[clyde].dir=left;
                        return g;
                    } 


                    if(g.lab[gy-1][gx] != '#' && dir != down)
                        dist[up]=distpontos(gy-1, gx, alvo);
                    else
                        dist[up]=MAIORDIST;

                    if(g.lab[gy][gx-1] != '#' && dir != right)
                        dist[left]=distpontos(gy, gx-1, alvo);
                    else 
                        dist[left]=MAIORDIST;

                    if(g.lab[gy+1][gx] != '#' && g.lab[gy+1][gx] != '-' && dir != up)
                        dist[down]=distpontos(gy+1, gx, alvo);
                    else
                        dist[down]=MAIORDIST;

                    if(g.lab[gy][gx+1] != '#' && dir != left)
                        dist[right]=distpontos(gy, gx+1, alvo);
                    else
                        dist[right]=MAIORDIST;

                    for(i=0; i<4; i++)
                        if(dist[i] < menordist)
                        {
                            menordist=dist[i];
                            auxdir=i;
                        }

                    switch(auxdir)
                    {
                        case up :   --g.ghost[clyde].pos.y;
                                    g.ghost[clyde].dir=up;
                                    break;

                        case left :  --g.ghost[clyde].pos.x;
                                     g.ghost[clyde].dir=left;
                                     break;

                        case down :  ++g.ghost[clyde].pos.y;
                                     g.ghost[clyde].dir=down;
                                     break;

                        case right :  ++g.ghost[clyde].pos.x;
                                      g.ghost[clyde].dir=right;
                                      break;
                    }


                    return g;
    }

    if(g.ghost[clyde].pos.x <=0) /*teleporte dos ghosts da esquerda*/
        g.ghost[clyde].pos.x =19;
    if(g.ghost[clyde].pos.x >=19) 
        g.ghost[clyde].pos.x =0;

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function to color the ghosts
 * @responsible for coloring the ghosts in their different modes
 * @return void
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

void coresfant(t_game g)
{
    int f;

    /*print cores fantasmas*/
    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        if(g.ghost[f].mode == chase || g.ghost[f].mode == scatter)
            mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, f + 1, NULL); /* cor fantasmas modos chase e scatter */
        else
            if(g.ghost[f].mode == afraid)
                mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 5, NULL); /* cor fantasmas modo afraid */
            else
                if(g.ghost[f].mode == dead)
                    mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 9, NULL); /* cor fantasmas modo dead */
    }
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function to print the score
 * @responsible for marking and setting the score on the game screen
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

t_game score(t_game g)
{ 
    int px, py, f;

    px = g.pacman.pos.x;
    py = g.pacman.pos.y;

    g.pacman.revert++;
    g.pacman.cont++; /*contador para velocidade do fantasma no modo afraid*/
    if(g.lab[py][px] == 'o') /* pellet especial */
    {
        g.pacman.tFim = time(NULL);
        g.pacman.tInicio = time(NULL);
        g.pacman.tDecorrido = difftime(g.pacman.tFim,g.pacman.tInicio);
        g.pacman.mult=1;
        g.pacman.dots++;
        g.pacman.cdots++;
        g.pacman.revert=0;
        g.lab[py][px] = ' ';
        g.pacman.score += 50;
        for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
            if(g.ghost[f].mode!=dead)
                g.ghost[f].mode=afraid;
    }
    for(f=blinky;f<=clyde;f++)
        if(g.ghost[f].mode==afraid)
            if(g.pacman.tDecorrido>7-(float)g.pacman.fase*0.35)
            {
                g.pacman.mult=1;
                g.ghost[f].mode=scatter;
            }

    if(g.lab[py][px] == '.') /* pellet comum */
    {
        g.pacman.dots++;
        g.pacman.cdots++;
        g.lab[py][px] = ' ';
        g.pacman.score += 10;
    }

    /*print da cherry */
    if(g.pacman.cdots==52 || g.pacman.cdots==127)
        g.lab[13][9] = '%';

    /* pontuacao por cherry */
    if(g.lab[13][9]=='%' && g.pacman.pos.y==13 && g.pacman.pos.x==9)
    {
        g.lab[13][9]=' ';
        g.pacman.score += 100;
    }

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function responsible for changing phases
 * @function that restarts the phases in relation to the end game in which
 * @return t_game
 * @author upe-blink
 * @version 20171220.1.1
 * @date 2017-12-20
 */

t_game passfase(t_game g)
{
    int f, y;

    /*atualizacao de quantidade de pellets no mapa*/
    g.pacman.tdots=0;
    for(y=0;y<LABL;y++)
        for(f=0;f<LABC;f++)
            if(g.lab[y][f]=='.'||g.lab[y][f]=='o')
                g.pacman.tdots++;

    /*se nao resta mais pellets no mapa*/
    if(g.pacman.tdots==0)
    {
        g.pacman.fase++;
        g=newfase(g);
    }

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function responsible for rebooting the game
 * @function that reboots the game when in new phase
 * @return t_game
 * @author upe-blink
 * @version 20171220.1.1
 * @date 2017-12-20
 */

t_game newfase(t_game g)
{
    int f, y;

    for(y = 0; y < LABL; y++)
        strcpy(g.lab[y], labmodel[y]); /* copy the labyrinth (lines of strings) */

    g.pacman.pos.y = 17; /* pacman start position (line) */
    g.pacman.pos.x = 9; /* pacman start position (column) */
    g.pacman.dir = left; /* pacman start direction */
    g.pacman.tenta = 1; /* pacman start int tenta */
    g.pacman.dots=0; /*numero de dots para inicializacao dos fantasmas*/
    g.pacman.cdots=0;  /*numero de dots para inicializacao da cherry*/

    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        switch(f)
        {
            case blinky:
                g.ghost[f].pos.y= 7; /* blinky start position (line) */
                g.ghost[f].pos.x= 9; /* blinky start position (column) */
                g.ghost[f].starget.y= 0; /* preferred corner (line) */
                g.ghost[f].starget.x= 19; /* preferred corner (column) */
                break;
            case pinky:
                g.ghost[f].pos.y = 9; /* pinky start position (line) */
                g.ghost[f].pos.x = 10; /* pinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
                break;
            case inky:
                g.ghost[f].pos.y = 10; /* inky start position (line) */
                g.ghost[f].pos.x = 10; /* inky start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case clyde:
                g.ghost[f].pos.y = 11; /* clyde start position (line) */
                g.ghost[f].pos.x = 10; /* clyde start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
        }
        g.ghost[f].dir = left; /* start direction and future direction: left */
        g.ghost[f].mode = scatter; /* start mode*/
    }

    g.lab[17][9] = ' '; /* otimizacao da remocao do pacman inicial */
    g.lab[7][9] = '.'; /* remocao do blinky inicial */
    g.lab[9][10] = ' '; /*remocao do pinky inicial */
    g.lab[10][10] =' '; /* remocao do inky inicial */
    g.lab[11][10] =' '; /* remocao do cleyde inicial */

    printlab(g);
    sleep(2);

    return g;
} 

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function that will check the amount of lives
 * @function responsible for checking the amount of lives
 * @return int
 * @author upe-blink
 * @version 20171112.1.1
 * @date 2017-12-11
 */

int breakgame(t_game g)
{
    int j;
    /*fim de jogo por 0vidas*/
    if(g.pacman.life==0)
        j=0;
    else
        j=1;

    return j;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function that will initialize some pre-requisites for the game to work
 * @function responsible for saving the amount of lives, pacman's initial position, as well as initializing functions of the ncurses library
 * @return t_game
 * @author upe-blink
 * @version 20171112.1.1
 * @date 2017-12-11
 */

t_game upecman_init(void)
{
    IFDEBUG("init()");
    /* initialization */

    t_game g;
    int f, y;

    for(y = 0; y < LABL; y++)
        strcpy(g.lab[y], labmodel[y]); /* copy the labyrinth (lines of strings) */

    g.pacman.pos.y = 17; /* pacman start position (line) */
    g.pacman.pos.x = 9; /* pacman start position (column) */
    g.pacman.dir = left; /* pacman start direction */
    g.pacman.tenta = 1; /* pacman start int tenta */
    g.pacman.fase = 1; /* pacman start int fase */
    g.pacman.life = 3; /* 3 lifes */
    g.pacman.score = 0; /* 0 points */
    g.pacman.dots=0;  /*numero de pllets comidos para fantasmas init*/
    g.pacman.cdots=0;  /*numero de pllets comidos para cherry init*/

    /*numero inicial de pellets no mapa*/
    g.pacman.tdots=0;
    for(y=0;y<LABL;y++)
        for(f=0;f<LABC;f++)
            if(g.lab[y][f]=='.'||g.lab[y][f]=='o')
                g.pacman.tdots++;

    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        switch(f)
        {
            case blinky:
                g.ghost[f].pos.y = 7; /* blinky start position (line) */
                g.ghost[f].pos.x = 9; /* blinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case pinky:
                g.ghost[f].pos.y = 9; /* pinky start position (line) */
                g.ghost[f].pos.x = 10; /* pinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
                break;
            case inky:
                g.ghost[f].pos.y = 10; /* inky start position (line) */
                g.ghost[f].pos.x = 10; /* inky start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case clyde:
                g.ghost[f].pos.y = 11; /* clyde start position (line) */
                g.ghost[f].pos.x = 10; /* clyde start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
        }
        g.ghost[f].dir = left; /* start direction and future direction: left */
        g.ghost[f].mode = scatter; /* start mode*/
    }

    initscr(); /* start ncurses mode screen */
    cbreak(); /* stop line buffering */
    keypad(stdscr, TRUE); /* grab extended keys */
    noecho(); /* stop echoing chars */
    timeout(0); /* do not wait */
    start_color(); /* initialize the color functionality */

    /* Initialize all the colors */
    init_pair(1,COLOR_RED,COLOR_BLACK); /* Blinky */
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); /* Pinky */
    init_pair(3, COLOR_CYAN, COLOR_BLACK); /* Inky */
    init_pair(4, COLOR_GREEN, COLOR_BLACK); /* Clyde doesn't have orange :( */
    init_pair(5, COLOR_WHITE, COLOR_BLUE); /* Afraid mode: blue with A_BLINK */
    init_pair(6,COLOR_YELLOW,COLOR_BLACK); /* Pacman */
    init_pair(7, COLOR_RED, COLOR_GREEN); /* Cherry */
    init_pair(8, COLOR_BLUE, COLOR_BLACK); /* Lab Walls */
    init_pair(9, COLOR_BLACK, COLOR_WHITE); /* Dead mode */

    g.lab[17][9] = ' '; /* otimizacao da remocao do pacman inicial */
    g.lab[7][9] = '.'; /* remocao do blinky inicial */
    g.lab[9][10] = ' '; /*remocao do pinky inicial */
    g.lab[10][10] =' '; /* remocao do inky inicial */
    g.lab[11][10] =' '; /* remocao do cleyde inicial */

    return g;
}  

/* ---------------------------------------------------------------------- */

/** 
 * @ingroup upe-blink
 * @brief function that reboots the game
 * @function responsible for rebooting the game when pacman dies
 * @return t_game
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-20
 * */

t_game p_init(t_game g)
{    
    int f;

    sleep(1);
    g.pacman.dots=0;
    g.pacman.pos.y = 17; /* pacman start position (line) */
    g.pacman.pos.x = 9; /* pacman start position (column) */
    g.pacman.dir = left; /* pacman start direction */
    g.pacman.tenta = 1; /* pacman start int tenta */

    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        switch(f)
        {
            case blinky:
                g.ghost[f].pos.y= 7; /* blinky start position (line) */
                g.ghost[f].pos.x= 9; /* blinky start position (column) */
                g.ghost[f].starget.y= 0; /* preferred corner (line) */
                g.ghost[f].starget.x= 19; /* preferred corner (column) */
                break;
            case pinky:
                g.ghost[f].pos.y = 9; /* pinky start position (line) */
                g.ghost[f].pos.x = 10; /* pinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
                break;
            case inky:
                g.ghost[f].pos.y = 10; /* inky start position (line) */
                g.ghost[f].pos.x = 10; /* inky start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case clyde:
                g.ghost[f].pos.y = 11; /* clyde start position (line) */
                g.ghost[f].pos.x = 10; /* clyde start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
        }
        g.ghost[f].dir = left; /* start direction and future direction: left */
        g.ghost[f].mode = scatter; /* start mode*/
    }

    printlab(g);
    sleep(1);

    return g;
}

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function that prints the end screen
 * @function responsible for printing the game over screen when game is lost
 * @return Void
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-20
 */

void printover(t_game g)
{
    curs_set(0); /*Esconde cursor*/
    clear(); /*clear screen*/

    /*MSG game over*/
    mvprintw(3, 14,"GAME OVER");
    mvchgat(3, 14, 9, A_BOLD, 9, NULL);

    /*MSG SCORE*/
    mvprintw(4, 14, "SCORE:%d", g.pacman.score); /*Printa o score na tela*/
    mvchgat(4, 14, 6, A_BOLD, 1, NULL); /*pinta a palavra score */

    /*MSG nome do desenvolvedor*/
    mvprintw(6, 14,"UPEBLINK");
    mvchgat(6, 14, 3, A_BOLD, 8, NULL);
    mvchgat(6, 17, 5, A_BOLD, 1, NULL);

    /*MSG PACMAN*/
    mvprintw(7, 15,"PACMAN");
    mvchgat(7, 15, 6, A_BOLD, 6, NULL);

    /*MSG jogar novamente*/
    mvprintw(9, 6,"Do You want to play again?");
    mvprintw(10, 11,"y->YES or n->NO");

}    

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function that prints the map of the game
 * @function responsible for printing the movement of each ghost, as well as pacman, in addition to the score and remove the cursor
 * @return Void
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-11
 */

void printlab(t_game g)
{
    IFDEBUG("printlab()");

    int y, f = 0;

    curs_set(0);/*Esconder cursor*/
    clear(); /* clear the screen */

    /*printa o labirinto*/
    for(y = 0; y < LABL; y++)
        printw("%s\n", g.lab[y]);

    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
        mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, f + 1, NULL);

    for(y = 0; y < LABL; y++)
        for(f = 0; f < LABC; f++)      /*pinta as bordas do mapa*/
            if(g.lab[y][f] == '#')
                mvchgat(y, f, 1, A_BOLD, 8, NULL);

    mvprintw(g.pacman.pos.y, g.pacman.pos.x, "@");  /*print da movimentacao do pacman*/
    mvchgat(g.pacman.pos.y, g.pacman.pos.x, 1, A_BOLD, 6, NULL); /*cor do pacman acompanhando movimentacao*/

    /*print caractere de cada fantasma*/
    mvprintw(g.ghost[0].pos.y, g.ghost[0].pos.x, "B"); /* print da movimentacao do blinky */
    mvprintw(g.ghost[1].pos.y, g.ghost[1].pos.x, "P"); /* print da movimentacao do pinky */
    mvprintw(g.ghost[2].pos.y, g.ghost[2].pos.x, "I");  /*print da movimentacao do inky*/
    mvprintw(g.ghost[3].pos.y, g.ghost[3].pos.x, "C");  /*print da movimentacao do fantasma clyde*/

    coresfant(g);

    if(g.lab[13][9] == '%')
        mvchgat(13, 9, 1, A_BOLD, 7, NULL); /*cor cereja*/

    mvprintw(4, 25, "SCORE:%d", g.pacman.score); /*Printa o score na tela*/
    mvchgat(4, 25, 6, A_BOLD, 1, NULL); /*pinta a palavra score */
    mvprintw(20, 25, "Press 'q' to quit."); /*Comando de parada do programa*/
    mvchgat(20, 25, 19, A_BOLD, 4, NULL); /*Colere a press 'q'...*/

    mvprintw(1, 25, "LIFE: %d", g.pacman.life); /*"print vida do Pacman"*/
    mvchgat(1, 25, 5, A_BOLD, 1, NULL); /* colore a life */

    mvprintw(6, 25, "FASE: %d", g.pacman.fase); /*"print fase do Pacman"*/
    mvchgat(6, 25, 5, A_BOLD, 1, NULL); /* colore a fase */

    /*numero de pellets*/
    mvprintw(23, 0, "Total Pellets: %d", g.pacman.tdots); /*quantidade de pellets no mapa*/
    mvchgat(23, 0, 18, A_BOLD, 1, NULL); /* colore a informacao do Total pellets */

    /* printa a quantidade de vidas e colore */
    if(g.pacman.life > 0)
    {
        mvprintw(2, 25, "@");
        mvchgat(2, 25, 1, A_BOLD, 6, NULL);
    } 

    if(g.pacman.life > 1)
    {
        mvprintw(2, 27, "@");
        mvchgat(2, 27, 1, A_BOLD, 6, NULL);
    } 

    if(g.pacman.life > 2)
    { 
        mvprintw(2, 29, "@");
        mvchgat(2, 29, 1, A_BOLD, 6, NULL);
    }

    refresh();
} 

/* ---------------------------------------------------------------------- */

/**
 * @ingroup upe-blink
 * @brief function that calculates a distance
 * @function responsible for calculating the distance for the andaclyde function
 * @return int
 * @author upe-blink
 * @version 20171211.1.1
 * @date 2017-12-20
 */

double distpontos(int gy, int gx, t_pos alvo)
{
    double distancia;

    distancia = sqrt(pow((gx-alvo.x),2)+pow((gy-alvo.y),2));

    return distancia;
} 

/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20160612.142044      */

