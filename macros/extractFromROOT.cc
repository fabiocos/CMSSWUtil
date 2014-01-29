//
// Extract one (or more histograms from a monitor ROOT file
// Benigno 20050617
//
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "TROOT.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPostScript.h"


void searchandplot( TDirectory *d, std::string objname, std::string outdir, std::string outtype, TCanvas *c ) {
 
  TIter i( d->GetListOfKeys() );
  TKey *k;
  while( (k = (TKey*)i())) {
    if( k->GetClassName() == TString( "TDirectory" ) ) {
      TDirectory *d = (TDirectory*)k->ReadObj();
      d->cd();
      TObject* obj = d->Get( objname.c_str() );
      if( obj != NULL ) {
	std::string gifname = outdir + "/" + objname + outtype;
	if( dynamic_cast<TH1F*>(obj) ) dynamic_cast<TH1F*>(obj)->Draw();
	if( dynamic_cast<TH2F*>(obj) ) dynamic_cast<TH2F*>(obj)->Draw("colz");
	if( dynamic_cast<TGraph*>(obj) ) dynamic_cast<TGraph*>(obj)->Draw();
	c->Update();
	c->SaveAs( gifname.c_str() );
      }
    }
  }
}
      

void add_data(  TDirectory *d, std::vector<TH1F*> &hist1d, std::vector<TH2F*> &hist2d, std::vector<TGraph*> &graphs  ) {

  TIter i( d->GetListOfKeys() );
  TKey *k;
  while( (k = (TKey*)i())) {
    if( k->GetClassName() == TString( "TH1F" ) ) {
      hist1d.push_back( (TH1F*)k->ReadObj() );
      
    }
    else if( k->GetClassName() == TString( "TH2F" ) ) {
      hist2d.push_back( (TH2F*)k->ReadObj() );
    }
    else if( k->GetClassName() == TString( "TGraph" ) ) {
      graphs.push_back( (TGraph*)k->ReadObj() );
    }
    else if( k->GetClassName() == TString( "TDirectory" ) ) {
      TDirectory *d = (TDirectory*)k->ReadObj();
      d->cd();
      add_data( d, hist1d, hist2d, graphs );
    }
    else {
      std::cout << "Ignored """ << k->GetName() << """, as of unknown TClass " << k->GetClassName() << std::endl;
    }
  }
}

void usage( char* cp ) {

  std::cout << 
"\n\
usage: " << cp << " [-h] [-j][-n object_name] [-s size] root_file_name \n\n\
     g             : output to a GIF file \n\
     h             : print this help message \n\
     j             : output to a JPEG file (default)\n\
     n object_name : plot to a gif file just 'object_name' ROOT object \n\
     o output_dir  : (create the dir. and) output the files there\n\
     s size        : canvas size \n\n";
}


int main( int argc, char **argv ) {

  std::string outdir( "./outfiles/" );

  std::string outtype = ".jpg";
  std::string root_file;

  std::vector<std::string> objname;
  bool        objsel = false;    

  int         csize = 250;

  int         rc;
  char        *cp;

  if(( cp = (char*) strrchr( argv[0], '/' )) != NULL ) {
    ++cp;
  }
  else {
    cp = argv[0];
  }

  if( argc > 1 ) {
    while(( rc = getopt( argc, argv, "ghjn:o:s:" )) != EOF ) {
      switch( rc ) {
      case 'g':
	outtype = ".gif";
	break;
      case 'h':
        usage(cp);
	return(0);
	break;
      case 'j':
	outtype = ".jpg";
	break;
      case 'n':
	objname.push_back( optarg );
	objsel  = true;
        break;
      case 'o':
	outdir = optarg;
        break;
      case 's':
	csize = atoi(optarg);
        break;
      case '?':
        return(-1);
	break;
      default:
	break;
      }
    }  
  }
  else {
    usage( cp );
    return (-1);
  }
  
  if( optind < argc ) {
    root_file = argv[optind];
  }
  else {
    std::cerr << "No ROOT filename specified" << std::endl;
    return (-1);
  }

  gROOT->SetBatch( true );


  std::vector<TH1F*> hist1d;
  std::vector<TH2F*> hist2d;
  std::vector<TGraph*> graphs;

  TFile *f = new TFile( root_file.c_str(), "r" );

  TCanvas *c = new TCanvas( "c", "output", csize , csize ); 

  mode_t mask = ~(S_IRWXU|S_IRWXG|S_IRWXO);
  umask( mask );
  if( mkdir( outdir.c_str(), S_IRWXU|S_IRWXG|S_IRWXO ) != 0 ) {
    //perror( argv[0] );
  }

  if( objsel ) {
    for( unsigned int k=0; k<objname.size(); k++ ) {
      TObject* obj = f->Get( objname[k].c_str() );
      if( obj != NULL ) {
	std::string gifname = outdir + "/" + objname[k] + outtype;
	if( dynamic_cast<TH1F*>(obj) ) dynamic_cast<TH1F*>(obj)->Draw();
	if( dynamic_cast<TH2F*>(obj) ) dynamic_cast<TH2F*>(obj)->Draw("colz");
	if( dynamic_cast<TGraph*>(obj) ) dynamic_cast<TGraph*>(obj)->Draw();
	c->Update();
	c->SaveAs( gifname.c_str() );
      }
      else {
	searchandplot( f, objname[k], outdir, outtype, c );
      }
    } 
  }
  else {
    add_data( f, hist1d, hist2d, graphs );
    for( unsigned int i=0; i<hist1d.size(); i++ ) {
      std::string name =  hist1d[i]->GetName();
      std::string gifname = outdir + "/" + name + outtype;
      hist1d[i]->Draw();
      c->Update();
      c->SaveAs( gifname.c_str() );
    }
    for( unsigned int i=0; i<hist2d.size(); i++ ) {
      std::string name =  hist2d[i]->GetName();
      std::string gifname = outdir + "/" + name + outtype;
      hist2d[i]->Draw("colz");
      c->Update();
      c->SaveAs( gifname.c_str() );
    }
    for( unsigned int i=0; i<graphs.size(); i++ ) {
      std::string name =  graphs[i]->GetName();
      std::string gifname = outdir + "/" + name + outtype;
      graphs[i]->Draw();
      c->Update();
      c->SaveAs( gifname.c_str() );
    }
  } 
  
  return 0;
}
