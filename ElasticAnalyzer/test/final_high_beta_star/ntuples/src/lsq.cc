#include "TGraphErrors.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TRandom3.h"

TRandom3 myrandom_variable ;

void renorm_group_equations()
{

}

void van_der_waals(double R, double T, double v, double b, double a)
{
	double p = ((R * T) / (v - b)) - (a / (v*v)) ;
}

void yukawa(double r, double g, double alpha, double m)
{
	double V = -(g*g) * (exp(-alpha * m * r) / r);
}

void lsq()
{
   const int myseed = 12 ;

	myrandom_variable.SetSeed(myseed) ;

	TGraphErrors *graph = new TGraphErrors() ;
	
	const int number_of_measurements = 10 ;
	const int number_of_parameters = 2 ;

	TVectorD yv(number_of_measurements) ;
	TMatrixD VI(number_of_measurements, number_of_measurements) ;

	TMatrixD A(number_of_measurements, number_of_parameters) ;

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
		
		A(i, 0) = 1 ;
		A(i, 1) = i ;
	}

	TVectorD theta(number_of_parameters), y_predicted(number_of_measurements), thetae(number_of_parameters) ;
	
	y_predicted = A*theta ;
	
	TMatrixD AT(number_of_parameters, number_of_measurements) ;
	AT.Transpose(A) ;

	TMatrixD ATVIAI(number_of_parameters, number_of_parameters) ;
   ATVIAI = (AT*VI*A).Invert() ;
	
	thetae = (ATVIAI*AT*VI)*yv ;
	thetae.Print() ;
	
	graph->Fit("pol1") ;

	TCanvas c ;

	graph->SetMarkerStyle(20) ;
	graph->Draw("ap") ;
	c.SaveAs("fig/c.pdf") ;
}

int main()
{
	van_der_waals(1, 2, 3, 4, 5) ;
	yukawa(1, 2, 3, 4) ;
   lsq() ;
}
