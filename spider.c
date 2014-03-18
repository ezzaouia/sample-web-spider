#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char* _DumpSourcePageEntree( char* PageHtmlUrl ) ;
void _DumpSourceLesAutresPages( char PageHtmlUrl[BUFSIZ] , char PageHtmlPath[BUFSIZ]);
char* _DumpSourcePageRobotsTxt( char* PageHtmlUrl ); 
char* _HrefPageHtmlTmp( char* PageHtmlPath );
char* _HrefPageHtmlPermis( char* PageHtmlUrl , char* HrefPageHtmlTmpPath , char* RobotTxtPath );


int main( int argc, char** argv )
{
	char buffer[512] ;
	
	_DumpSourcePageRobotsTxt( argv[1] );
	_DumpSourcePageEntree( argv[1] ) ;
	
	_HrefPageHtmlPermis( argv[1] , _HrefPageHtmlTmp( "site/index.html" ) , "site/robots.txt" );
	
	_DumpSourceLesAutresPages( argv[1] , "site/hypertext.txt" );
	
	//system(" cut -d'\"' -f2 resulta.txt | egrep -v -e ^[h0123456789] -v -e .pdf$ > resulta0.txt");
	//_DumpSourceLesAutresPages( argv[1]  ,"site/hypertext.txt");
	
	
FILE * tubePage = NULL;
	
	tubePage = fopen("resulta0.txt", "r");

//boucle:
	//fseek ( tubePage , 0L , SEEK_SET );
	while(!feof(tubePage))
	{
		fscanf(tubePage , "%s\n", buffer);
		//puts(buffer);
		_DumpSourceLesAutresPages( argv[1] , _HrefPageHtmlPermis( argv[1] , _HrefPageHtmlTmp( buffer )  , "site/robots.txt" ) );
		
		
	}
	
	//system(" cut -d'\"' -f2 resulta.txt | egrep -v -e ^[h0123456789] -v -e .pdf$ > resulta0.txt");
	
	//goto boucle;
	
	fclose(tubePage);
	
return 0;
}

// pour charger le fichier robots.txt
char* _DumpSourcePageRobotsTxt( char PageHtmlUrl[BUFSIZ] ) 
{
	 
	 char bufferWget[BUFSIZ] = "wget  -P site  -nc -e robots=off --wait 1 ";
	 //char c;
	 // char buff[512];
	 FILE * Fich;
	 
	 strcat( strcat( bufferWget , PageHtmlUrl ) , "/robots.txt" );
	  
	if ((Fich = popen( bufferWget , "r")) == NULL)
 	{ perror("popen"); exit(1); }
 
 //fseek ( Fich , 0L , SEEK_SET );
  //while (( c =fgetc(Fich)) != -1) 
  //putchar(c);
  pclose(Fich); 

	 
	
	 
	 //system( bufferWget ); // je dois voir l'enreg se fait où sinon une rederection est daroori
		
return "site/robots.txt" ;
}

// charger le code source de la page d'entrée
char* _DumpSourcePageEntree( char* PageHtmlUrl ) 
{
	
	char bufferWget[BUFSIZ] = "wget -P site  -nc -e robots=off --wait 1 ";
	strcat( bufferWget , PageHtmlUrl );
	//puts(bufferWget);
	FILE * Fich;
	//char c;
	if ((Fich = popen( bufferWget , "r")) == NULL)
 	{ perror("popen"); exit(1); }
 
 //fseek ( Fich , 0L , SEEK_SET );
  //while (( c =fgetc(Fich)) != -1) 
 // putchar(c);
  pclose(Fich); 
	//system( bufferWget ); // je dois voir l'enreg se fait où sinon une rederection est daroori
	
	//fclose( PageHtmlSource );
	
return "site/index.html" ;
}

// pour charger les pages dont l'url figure dans la page d'entree
void _DumpSourceLesAutresPages(  char PageHtmlUrl[BUFSIZ]  , char PageHtmlPath[BUFSIZ] )
{
	int indicateur;char c;
   char bufferHyperLien[BUFSIZ];
   char bufferCmp[BUFSIZ];
   char bufferUrl[BUFSIZ];
   char bufferNomDom[BUFSIZ];
   char bufferSlash[2] ;
  // char bufferWget[BUFSIZ] ;
   int length = strlen( PageHtmlUrl ) - 1 ;
   
   


	FILE* HyperTextOut = NULL;
	FILE* AutrePage = NULL;
	FILE* HyperTextOutTotal = NULL;
	
	HyperTextOutTotal = fopen("site/hypertextTotal.txt", "a+");
	HyperTextOut = fopen( PageHtmlPath , "r");
	
	// cette boucle pour eviter qu'un url se repete
	do
	{
		indicateur = 0;
		fscanf(HyperTextOut , "%s\n", bufferHyperLien);
		//puts(bufferHyperLien);
		
		fseek ( HyperTextOutTotal , 0L , SEEK_SET );
		
		while(!feof(HyperTextOutTotal))
		{
			
			fscanf(HyperTextOutTotal , "%s\n", bufferCmp );
			
			if( strcmp(bufferHyperLien , bufferCmp ) == 0  )
			{
			
				//printf("%s\n%s ---  non\n",buffer,bufferCmp);
				indicateur = 1;
				break;
			}
			/*else
			{
				printf("%s\n%s ---  oui\n",buffer,bufferCmp);
				break;
			}*/
			
		}
		
		/*
		if( indicateur == 1 )
		{
			;//fprintf(HyperTextOut , "%s non\n", buffer);
		}
		*/
		
		if( indicateur == 0 )
		{
			fprintf(HyperTextOutTotal , "%s\n", bufferHyperLien);
			//puts(bufferHyperLien);
		}
		
	}while(!feof(HyperTextOut));
	
	//----------------
	// on commence par le debut du fichier
	fseek ( HyperTextOutTotal , 0L , SEEK_SET );
	
	while( !feof(HyperTextOutTotal) )
	{
		fscanf( HyperTextOutTotal , "%s\n" , bufferUrl );
		//puts(bufferUrl);
		// on va charger tt les pages dont l'url figure dans la page d'entree
		// -e robots= off c pour eviter que wget ne prend en compte le fichier robots.txt 
		// on va le faire nous meme... après
		char bufferWget[BUFSIZ] = "wget -nv  -P site -nc -a resulta.txt -e robots=off --wait 1 ";
		
		
		//if( strncmp( bufferUrl , "http://", 7 ) != 0)
		//{
			strcpy(bufferNomDom , PageHtmlUrl);
			strcpy(bufferSlash , "/");
			
			// chemin relatif
			if( bufferUrl[0] == '/' )
			
				//strcat( bufferWget , strcat( bufferNomDom , bufferUrl ) ) ;
			strcat( bufferNomDom , bufferUrl );
			else if( ( bufferUrl[0] != '/') && (( strncmp( bufferUrl , "http://", 7 ) != 0 )  ))
			{
				strcat( bufferNomDom ,  strcat( bufferSlash , bufferUrl ) );
				//strcat( bufferWget , bufferNomDom);
				//puts(bufferWget);
			}
			else 
			strcpy(bufferNomDom , bufferUrl);
			
			
			
		//}
		// tester q'on a pas sortie du nom d domaine
		if( strncmp( bufferNomDom , PageHtmlUrl , length ) == 0 )
		{
			strcat( bufferWget , bufferNomDom );
			puts(bufferWget);
			
			if (( AutrePage = popen( bufferWget , "r" )) == NULL)
	 		{
	 	   	perror("popen");
	 	      exit(1);
	 	   }pclose(AutrePage);
		}
		
		*bufferNomDom = "";
		*bufferSlash = "";
 		//
 		
			
 		
	}
	
	
	fclose(HyperTextOut);
	fclose(HyperTextOutTotal);
	// rederection des noms des pages chargées vers resultat0.txt pour refaire le travail recursivement  
	system(" cut -d'\"' -f2 resulta.txt | egrep -v -e ^[h0123456789] -v -e .pdf$ > resulta0.txt");
	//remove("resulta.txt");
	
}

// extraire les href de façon avgle 
char* _HrefPageHtmlTmp( char* PageHtmlPath )
{

	char c;
	int i;
	char bufferHref[4];	
	FILE* PageHtmlFichIn = NULL;
	FILE* HyperTextOutTmp = NULL;
	
	PageHtmlFichIn = fopen( PageHtmlPath , "r");
	HyperTextOutTmp = fopen("hypertextTmp.txt", "w");
	
	while( ( c = fgetc(PageHtmlFichIn) ) != -1 )
	{
	
		if( c == '<')
		{
			while( ( c = fgetc(PageHtmlFichIn) ) == ' ')
			{
				;
			}
				
				if( c == 'a')
				{
				
					while( ( ( c = fgetc(PageHtmlFichIn) ) == ' ' ) || ( c != 'h' ) )
					{
						;
					}
					
					if( ( bufferHref[0] =  c ) == 'h')
					{
						//  un tableau de h à = (non inclu) et puis strcmp avec herf
						for( i = 1 ; i<4 ; i++)
						bufferHref[i] = fgetc(PageHtmlFichIn);
						
						
						if(strcmp( bufferHref , "href" ) == 0 )
						{
							while( ( c = fgetc(PageHtmlFichIn) ) != '"' )
							{
								;
							}
				
							while( ( c = fgetc(PageHtmlFichIn) ) != '"' )
							{
					
								fputc( c, HyperTextOutTmp);
						
							}
						
							fprintf(HyperTextOutTmp, "\n");
							//puts(buffer);
						
						}
												
						
					}
				}
		}		
	}
	
	fclose(HyperTextOutTmp);
	fclose(PageHtmlFichIn);
	
return "hypertextTmp.txt";
}

// extraire les href autorisés en comparant avec robot.txt
char* _HrefPageHtmlPermis( char* PageHtmlUrl , char* HrefPageHtmlTmpPath , char* RobotTxtPath )
{
	int indicateur = 0 ;
   char bufferUrlRobot[BUFSIZ];
   char bufferUrlRobotAbsl[BUFSIZ];
   char bufferHyperLien[BUFSIZ];
   char bufferCmp[BUFSIZ];
   char bufferNomDom[BUFSIZ];
	char bufferSlash[BUFSIZ];

	FILE* HyperTextOutTmp = NULL;
	FILE* RobotsFich = NULL;
	FILE* HyperTextOut = NULL;
	
	HyperTextOutTmp = fopen( HrefPageHtmlTmpPath , "r");
	RobotsFich = fopen( RobotTxtPath , "r");
	HyperTextOut = fopen("site/hypertext.txt", "w");
	
	//fseek ( HyperTextOutTmp , 0L , SEEK_SET );
	do
	{
		indicateur = 0;
		fscanf(HyperTextOutTmp, "%s\n", bufferHyperLien);
		//puts(bufferHyperLien);
		
		fseek ( RobotsFich, 0L , SEEK_SET );
		
		while(!feof(RobotsFich))
		{
			
			fscanf(RobotsFich , "%s %s\n", bufferCmp , bufferUrlRobot);
			
			if( strncmp( bufferHyperLien , "http://", 7 ) != 0)
			{
			
				strcpy(bufferNomDom , PageHtmlUrl);
				strcpy(bufferSlash , "/");
			
					if( bufferHyperLien[0] == '/' )
			
						strcat( bufferNomDom , bufferHyperLien );
			
					else
					{
						strcat( bufferNomDom ,  strcat( bufferSlash , bufferHyperLien ));
				
						//puts(bufferWget);
					}
			
				//puts(bufferUrl);
				*bufferNomDom = "";
				*bufferSlash = "";
			}
			
			if( strncmp( bufferUrlRobot , "http://", 7 ) != 0)
			{
				strcpy(bufferNomDom , PageHtmlUrl);
				strcat( bufferNomDom , bufferUrlRobot );
				//puts(bufferNomDom);
				
			}
			
			
			if(( strcmp(bufferHyperLien , bufferNomDom ) == 0 ) && ( strcmp(bufferCmp, "Disallow:" ) == 0 ) )
			{
			
				//printf("%s\n%s ---  non\n",buffer,bufferCmp);
				indicateur = 1;
				break;
			}
			
			*bufferNomDom = "";
			
			/*else
			{
				printf("%s\n%s ---  oui\n",buffer,bufferCmp);
				break;
			}*/
			
		}
		
		/*
		if( indicateur == 1 )
		{
			;//fprintf(HyperTextOut , "%s non\n", buffer);
		}
		*/
		
		if( indicateur == 0 )
		{
			fprintf(HyperTextOut , "%s\n", bufferHyperLien);
		}
		
	}while(!feof(HyperTextOutTmp));
	
	fclose(RobotsFich);
	fclose(HyperTextOutTmp);
	fclose(HyperTextOut);
	
	// remove("hypertextTmp.txt");
	return "site/hypertext.txt";
}

