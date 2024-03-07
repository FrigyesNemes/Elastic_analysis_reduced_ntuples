#include "TGraphErrors.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TRandom3.h"

TRandom3 myrandom_variable ;

int main()
{
	TGraphErrors *graph = new TGraphErrors() ;
	
	const int number_of_measurements = 10 ;
	const int number_of_parameters = 2 ;

	TVectorD yv(number_of_measurements) ;

	TMatrixD A(number_of_measurements, number_of_parameters) ;

	for(int i = 0 ; i < number_of_measurements ; ++i)
	{
		double y = myrandom_variable.Gaus() ;
		yv(i) = y ;
		
		graph->AddPoint(i, y) ;
		graph->SetPointError(i, 0, 1) ;
		
		A(i, 0) = 1 ;
		A(i, 1) = i ;
	}

	TVectorD theta(number_of_parameters), y_predicted(number_of_measurements), thetae(number_of_parameters) ;
	
	y_predicted = A*theta ;
	
	TMatrixD AT(number_of_parameters, number_of_measurements) ;
	AT.Transpose(A) ;

	TMatrixD ATVIAI(number_of_parameters, number_of_parameters) ;
   ATVIAI = (AT*A).Invert() ;
	
	thetae = (ATVIAI*AT)*yv ;
	thetae.Print() ;
	
	graph->Fit("pol1") ;

	TCanvas c ;

	graph->SetMarkerStyle(20) ;
	graph->Draw("ap") ;
	c.SaveAs("fig/c.pdf") ;
}
