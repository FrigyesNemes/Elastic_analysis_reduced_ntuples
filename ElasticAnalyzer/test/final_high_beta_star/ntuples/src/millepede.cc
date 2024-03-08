#include "TGraphErrors.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TRandom3.h"

TRandom3 myrandom_variable ;

const int number_of_events = 10 ;

const int number_of_measurements = 10 ;
const int number_of_parameters = 2 ;

int initial_fit(TMatrixD *ap)
{
   const int myseed = 12 ;

  myrandom_variable.SetSeed(myseed) ;

  TGraphErrors *graph = new TGraphErrors() ;
  
  TVectorD yv(number_of_measurements) ;
  TMatrixD VI(number_of_measurements, number_of_measurements) ;

  TMatrixD *A = new TMatrixD(number_of_measurements, number_of_parameters) ;

  double sigma = 0.5 ;

  for(int i = 0 ; i < number_of_measurements ; ++i)
  for(int j = 0 ; j < number_of_measurements ; ++j)
  {
    VI(i,j) = 0.0 ;

    if(i == j) VI(i,j) = (sigma * sigma) ;
  }
  VI.Invert() ;
  VI.Print() ;


  for(int i = 0 ; i < number_of_measurements ; ++i)
  {
    double y = myrandom_variable.Gaus() ;
    yv(i) = y ;
    
    graph->AddPoint(i, y) ;
    graph->SetPointError(i, 0, 1) ;
    
    (*A)(i, 0) = 1 ;
    (*A)(i, 1) = i ;
  }

  TVectorD theta(number_of_parameters), y_predicted(number_of_measurements), thetae(number_of_parameters) ;
  
  y_predicted = (*A)*theta ;
  
  TMatrixD AT(number_of_parameters, number_of_measurements) ;
  AT.Transpose(*A) ;

  TMatrixD ATVIAI(number_of_parameters, number_of_parameters) ;
   ATVIAI = (AT*VI*(*A)).Invert() ;
  
  thetae = (ATVIAI*AT*VI)*yv ;
  thetae.Print() ;
  
  graph->Fit("pol1") ;

  TCanvas c ;

  graph->SetMarkerStyle(20) ;
  graph->Draw("ap") ;
  c.SaveAs("fig/c.pdf") ;
  
  ap = A ;
}


int alignment_fit()
{
}

int copy_matrix(TMatrixD *source, TMatrixD *target)
{
}

int main()
{
  TMatrixD large_matrix_A(number_of_events * number_of_parameters, number_of_events * number_of_parameters) ;
  TMatrixD *ap ;

  for(int i = 0 ; i < number_of_events ; ++i)
  {
    initial_fit(ap) ;
    
    copy_matrix(ap, &large_matrix_A) ;
    
    delete ap ;
  }
}
