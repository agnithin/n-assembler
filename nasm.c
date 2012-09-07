/*
N Assembler 1.0m
A simple two pass assembler convert
assembly language to machine language
date:10 mar 06
created by: 4th sem cse students from avv, bangalore
agnithin@yahoo.com
*/

#include<stdio.h>
#include<string.h>

/*mneumonics table///////*/
char mnu[20][8] = {"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT",
			"AREG","BREG","CREG","LT","LE","EQ","GT","GE","ANY"};
int opcode[2][20] = {0,1,2,3,4,5,6,7,8,9,10,1,2,3,1,2,3,4,5,6,  /*instn code*/
		     0,2,2,2,2,2,2,2,2,1,1 ,0,0,0,0,0,0,0,0,0}; /*no.of vars*/
/*///////////////////////*/

/*symbol table///////////*/
char sym[12][8];
int symcod[12],sy;
/*///////////////////////*/

/* file printing function */
void printfile(int f)
{
	FILE *file;
	/* select file according to arg passed */
	if(f==1)
	file=fopen("c:\\asm\\program.asm","r");
	else if(f==2)
	file=fopen("c:\\asm\\ir.txt","r");
	else if(f==3)
	file=fopen("c:\\asm\\code.txt","r");
	else if(f==4)
	file=fopen("c:\\asm\\instructions.txt","r");
	else
	return;

	if (file==NULL) /* chk if file not present */
	{
		printf("\nERROR : NO SOURCE FILE FOUND !\n\n");
		return;
	}

	while(1)
	{
		if(feof(file)) break;
		printf("%c",fgetc(file));
	}
	fclose(file);
	printf("\n");
}
/*fn to enter new assembly lang//////////////////////*/
void editpgm()
{
	FILE *file;
	int ch=1;
	char c;

	while(1)
	{
	clrscr(); /* display warning */
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("     ษออออออออออออออออออออออออออออ[ WARNING ! ]อออออออออออออออออออออออออออออป\n");
	printf("     บ                                                                      บ\n");
	printf("     บ      Do you want to enter a new assembly language program ?          บ\n");
	printf("     บ      WARNING : The existing program will be overwritten              บ\n");
	printf("     บ                                                                      บ\n");
	printf("     บ               YES	      		       NO                   บ\n");
	if(ch==0)
	printf("     บ               ~~~                                                    บ\n");
	else
	printf("     บ                                                ~~~                   บ\n");
	printf("     ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
	printf("\n\n\n\n\n\tNOTE : Please read instructions before writing a program");

	switch(getch())
		{
			case 75:  if(ch!=0) ch--;  break;   /* up key */
			case 77:  if(ch!=1) ch++;  break;   /* down key */
			case 13:                            /* enter key */
				if(ch==0)
				{
					file=fopen("c:\\asm\\program.asm","w");
					clrscr();
					printf("ษอออออออออออออออออออออออออออออ[ ASSEMBLY PROGRAM ]อออออออออออออออออออออออออออออป");
					printf("บ                                                                              บ");
					printf("\n\tNOTE : Enter '.' to end the program\n\n");
					scanf("%c",&c);
					while(c!='.')
						{
							fputc(c,file);
							scanf("%c",&c);
						}

					printf("บ                                                                              บ");
					printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
					getch();
					fclose(file);
					return;
				}
				else
					return;

		  }
	}
}

/* main processing function includes pass1 & pass2 ////////////////////////*/
void process()
{
	FILE *f1     /* assembly lang source program     */
	    ,*f2     /* intermediate code gen aftr pass1 */
	    ,*f3;    /* output machine code              */
	int i,flg=0,temp
	    ,ynsymtab=0 /* yes/no symbol table       : 0=n,1=y */
	    ,bastop=0   /* ptr before/after stop     : 0=b,1=a */
	    ,bainst=0   /* ptr before/after mnumonic : 0=b,1=a */
	    ,start      /* location counter */
	    ,var=0;     /* no. of vars for instn */
	char c,word[10]
	    ,buff[10]   /* buffer holding prev word */;

	sy=0; /* set symbol table ptr to 0 */
	clrscr();
	f1=fopen("c:\\asm\\program.asm","r");
	f2=fopen("c:\\asm\\ir.txt","w");

	fscanf(f1,"%s",word); /* chk if pgm starts with start */
	if(strcmp(word,"START") != 0 && strcmp(word,"ORIGIN") != 0)
	{
		printf("Illegal start to the program !\n\n");
		return;
	}
	fscanf(f1,"%d",&start); /* get location counter value */

	/* PASS I ########################################################*/
	while(!feof(f1))
	{       /* get word///////////////////////////////////////////////*/
		i=0;
		c=fgetc(f1);
		while((c != ' ') && (c != ',') && (c != '\n') && (c!='\t'))
		{
			word[i++] = c;
			c=fgetc(f1);
			if(feof(f1))
			break;
		}
		word[i++]='\0';
		if(strcmp(word,"\0")==0) /* skip if null string */
			continue;
		/*////////////////////////////////////////////////////////*/
		/*fscanf(f1,"%s",word);*/
		if(strcmp(word,"END")==0)  /*if end encountered stop pass 1*/
			break;

		/* chek if wrd present in mnumonic table //////////////////*/
		for(i=0;i<11;i++)
		{
			if(strcmp(word,&mnu[i][0])==0) /*if wrd present in mnu tab*/
			{
				if(var!=0) /*chk if correct no. of var enc in prev inst*/
				{
					printf("\nIllegal no of parameters (ERR COD: v-.%s.%d) !",buff,var);
					getch();
					return;
				}
				fprintf(f2,"\t%d\t+",start++);
				if(i==10)
						fprintf(f2,"\t%d",opcode[0][i]); /*print opcode of instn*/
				else
						fprintf(f2,"\t0%d",opcode[0][i]); /*print opcode of instn*/
				if(i==0) /* if stop stmt */
				{
					bastop=1;  /* ptr is aftr STOP stmt*/
					fprintf(f2,"\t0\t000");
				}
				flg=1;   /* wrd is an instn, dont chk for symbol */
				var=opcode[1][i]; /* no. of variables for instn */
				bainst=1;         /* ptr after instn */

				if(i==9 ||i==10) /* if read or print */
					fprintf(f2,"\t0");
				break;   /*break for loop*/
			}
		}
		/* chek if wrd is reg or condt code ///////////////////////*/
		for(i=11;i<20;i++)
		{
			if(strcmp(word,&mnu[i][0])==0) /*if wrd present in mnu tab*/
			{
				var--;
				fprintf(f2,"\t%d",opcode[0][i]); /*print opcode of instn*/
				flg=1;
				break;
			}
		}

		/* check for label ////////////////////////////////////////*/
		if(bainst==0  && flg==0 && bastop==0)
		{
			if(fgetc(f1)!=':')    /* if next symbol is not':' */
			{
				printf("\nIllegal symbol (%s) to begin a line !",word);
				getch();
				return;
			}
			symcod[sy]=start; /* add label to symbol tab */
			strcpy(sym[sy++],word); /* add lc value as address */
			continue;
		}

		/* check for declarative statement ////////////////////////*/
		if(strcmp(word,"DS")==0 || strcmp(word,"DC")==0) /*chk if wrd is  DC or DS*/
		{
			if(strcmp(word,"DS")==0)
				fscanf(f1,"%d",&temp);
			else
				fscanf(f1,"'%d'",&temp);
			for(i=0;i<sy;i++)   /* find word in symbol table */
				if(strcmp(sym[i],buff)==0)
					break;
			symcod[i]=start++; /* assign add as lc */
			while(temp!=1)
			{
				temp--;
				start++; /*allocation of var*/
			}
			continue;
		}

		/* if not any of the above add as symbol //////////////////*/
		if (flg==0)  /*if wrd not mnu add to sym tab*/
		{
			if(bainst && !bastop && --var<0) /*no.of varibles for prev instn*/
			{
				printf("\nIllegal no of parameters (ERR COD: v+.%s.%d) !",word,var);
				getch();
				return;
			}
			for(i=0;i<sy;i++)   /*ckh if word already present in symbol table */
				if(strcmp(sym[i],word)==0)
				{
					ynsymtab=1;  /*wrd present in sym tab */
					break;
				}
			if(!bastop) /* if ptr before STOP print symbol */
				fprintf(f2,"\t%s",word);
			if(!ynsymtab) /* if wrd not already present in sym tab */
				strcpy(sym[sy++],word); /* add to sym tab */

		}
		strcpy(buff,word); /* buffer holding prev sym if DC encountered */
		flg=0;  	   /* to chk if wrd is mnu or sym */
		ynsymtab=0;        /* to chk if sym already present in sym tab */
		if((c =='\n') && !bastop) /* if nxt char is '\n' print '\n' in ir */
		{
			fputc('\n',f2);
			bainst=0;
		}

	}
	fclose(f1);
	fclose(f2);
	/* END OF PASS I ##################################################*/


	/* PASS II ########################################################*/
	flg=0;
	f2=fopen("c:\\asm\\ir.txt","r");
	f3=fopen("c:\\asm\\code.txt","w");
	while(!feof(f2))
	{       /* scanning word /////////////////////////////////////////*/
		i=0;
		c=fgetc(f1);
		while((c != ' ') && (c != ',') && (c != '\n') && (c!='\t'))
		{
			word[i++] = c;
			c=fgetc(f1);
			if(feof(f1))
			break;
		}
		word[i++]='\0';
		if(strcmp(word,"\0")==0)
			continue;
		/*///////////////////////////////////////////////////////*/
		/*fscanf(f2,"%s",word);*/

		for(i=0;i<sy;i++)
		{
			if(strcmp(word,&sym[i][0])==0)/* if word present in sym tab */
			{
				fprintf(f3,"\t%d",symcod[i]);/* print address */
				flg=1;
				break;
			}
		}
		if(flg==0)/* if word not present in symtab */
			fprintf(f3,"\t%s",word);/* print as it is */
	if(c =='\n')  /* if nxt char is '\n' print '\n' in ir */
		fputc('\n',f3);
	flg=0;
	}
	fclose(f2);
	fclose(f3);

	/* END OF PASS II ################################################*/
	return;
}

/* menu //////////////////////////////////////////////////////////////////*/
void main()
{
	int ch=0,i;

	while(1)
	{
		clrscr(); /* 201ษ  205อ  186บ  187ป  188ผ  200ศ  204ฬ  185น */
		printf("ษอออออออออออออออออออออออออออออ[ N-ASSEMBLER 1.0 ]ออออออออออออออออออออออออออออออป");
		printf("บ                                                                              บ\n\n\t\t\t");
		if(ch==0) printf("=> ");
		printf("VIEW INSTRUCTIONS\n\n\t\t\t");
		if(ch==1) printf("=> ");
		printf("VIEW ASSEMBLY PROGRAM\n\n\t\t\t");
		if(ch==2) printf("=> ");
		printf("INPUT ASSEMBLY PROGRAM\n\n\t\t\t");
		if(ch==3) printf("=> ");
		printf("VIEW PASS I RESULTS\n\n\t\t\t");
		if(ch==4) printf("=> ");
		printf("VIEW PASS II RESULTS\n\n\t\t\t");
		if(ch==5) { printf("=> ");
		printf("E  X  I  T\n\n\n\n");  } else
		printf("EXIT\n\n\n\n");
		printf("    * Use up/down arrow keys to navigate, Press enter key to select\n");
		printf("บ                                                                              บ");
		printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

		switch(getch())
		{
			case 72:  if(ch!=0) ch--;  break;    /* up key */
			case 80:  if(ch!=5) ch++;  break;    /* down key */
			case 13:                             /* enter key */
				if(ch==0)  /* print instructions */
				{
					clrscr();
					printf("ษอออออออออออออออออออออออออออออออ[ INSTRUCTION ]ออออออออออออออออออออออออออออออออป");
					printf("บ                                                                              บ");
					printfile(4);
					printf("บ                                                                              บ");
					printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
					getch();
				}
				else if(ch==1) /* print assembly code */
				{
					clrscr();
					printf("ษอออออออออออออออออออออออออออออออออ[ PROGRAM ]ออออออออออออออออออออออออออออออออออป");
					printf("บ                                                                              บ");
					printfile(1);
					printf("บ                                                                              บ");
					printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
					getch();
				}
				else if(ch==2) /* enter new assembly pgm */
				     editpgm();

				else if(ch==3) /* result of pass 1 */
				{
					clrscr();
					if(sy==0);
						process();
					printf("ษอออออออออออออออออออออออออออออ[ INTERMEDIATE CODE ]ออออออออออออออออออออออออออออป");
					printf("บ                                                                              บ");
					printfile(2);
					printf("บ                                                                              บ");
					printf("ฬออออออออออออออออออออออออออออออ[ SYMBOL TABLE ]ออออออออออออออออออออออออออออออออน");
					printf("บ                                                                              บ");
					for(i=0;i<sy;i++)
					printf("\t%s\t%d\n",sym[i],symcod[i]);
					printf("บ                                                                              บ");
					printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
					getch();
			       }
			       else if(ch==4) /* gen machine code */
				{
					process();
					printf("ษออออออออออออออออออออออออออออออ[ MACHINE CODE ]ออออออออออออออออออออออออออออออออป");
					printf("บ                                                                              บ");
					printfile(3);
					printf("บ                                                                              บ");
					printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
					getch();
				}
				else if(ch==5) /* exit */
					exit(1);

		}
	}
}