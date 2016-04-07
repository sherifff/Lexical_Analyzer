#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "File_System.h"

//FILE * file;

File_System::File_System()
{
	//const char* name=fileName.c_str();
	/*errno_t err = fopen_s(&file,name,"r");
	if( err == 0 )
   {
	   printf( "The file ");
	   std::cout<< fileName;   
	  printf(" was opened\n" );
	  valid =true;
   }
   else
   {
      printf( "The file ");
	  std::cout<<fileName;  
	  printf(" was not opened\n" );
	  valid = false;
   }*/
	
	// = fopen( "‪go.txt", "r" );

	 /* if( !file ){	
	 // C4996
   // Note: fopen is deprecated; consider using fopen_s instead
      printf( "The file 'crt_fopen.c' was not opened\n" );
	  valid= false;
	}else{
      printf( "The file 'crt_fopen.c' was opened\n" );
	  valid= true;
	  //fprintf(file, "%s %s %s %d", "We", "are", "in", 2012);
	}
	



/*	//if( type == 'r' ){
		readFile.open("omar.txt");
		if( readFile.is_open() ){
			valid = true;
		}else{
			valid = false;
		}
	}else if(type == 'w'){
		writeFile->open(fileName);
		if( writeFile->is_open()){
			valid = true;
		}else{
			valid = false;
		}
	}
	*/
}


void File_System::init(){
	errno_t err = fopen_s (&file, "test2.txt", "r");
   if( err ){
		valid = false;
	}else{
		valid = true;
	}
}

File_System::~File_System(void)
{
}

std::string File_System::getLine(){
	std::string line;
	char *d;
	if( valid ){
		//cout << " PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP"<<endl;
		try{
	    fscanf_s(file,"%s",&d);
		}catch(exception e){
			cout << "throw";
		}
	}
	return "";
}

