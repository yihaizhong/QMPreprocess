#define USE_ELISE 0
#if USE_ELISE
#include "StdAfx.h"
#ifdef _DEBUG
#pragma comment(lib,"../../mm/lib/elised.lib")
#else
#pragma comment(lib,"../../mm/lib/elise.lib")
#endif


static int aNbH = 1 << 16;
int TheNbVois = 50;
int TheNbIter = 1;

void MakeFoncRepart(Im1D_REAL8 aH, int * aVMax = 0)
{
	double aNbP;
	ELISE_COPY(aH.all_pts(), aH.in(), sigma(aNbP));
	REAL8 * aDH = aH.data();
	for (int aK = 1; aK < aNbH; aK++)
	{
		if (aDH[aK] && aVMax) *aVMax = aK;
		aDH[aK] += aDH[aK - 1];
	}
	ELISE_COPY(aH.all_pts(), aH.in() * (255.0 / aNbP), aH.out());
}
int PreparSift_Main_yt(int argc, char ** argv)
{
	double aPEg = 1.0;
	double aPSrtEg = 3.0;
	double aPM = 2.0;


	std::string  aNameIn, aNameOut = "Sift.tif";

	ElInitArgMain
		(
		argc, argv,
		LArgMain() << EAMC(aNameIn, "Full name (Dir+Pattern)", eSAM_IsPatFile),
		LArgMain() << EAM(aNameOut, "NameOut", true)
		);

	Tiff_Im aTif = Tiff_Im::StdConvGen(aNameIn, 1, true);

	// Init Mems
	Pt2di aSz = aTif.sz();
	Im2D_U_INT2  anIm(aSz.x, aSz.y);

	Im1D_REAL8  aH(aNbH, 0.0);


	// Load image
	{
		Symb_FNum aFTif(aTif.in());
		ELISE_COPY(aTif.all_pts(), aFTif, anIm.out() | (aH.histo().chc(aFTif) << 1));
	}
	Im1D_REAL8  aHSqrt(aNbH, 0.0);
	ELISE_COPY(aH.all_pts(), sqrt(aH.in()), aHSqrt.out());



	int aVMax = 0;
	// Calcul Histo
	MakeFoncRepart(aH, &aVMax);
	MakeFoncRepart(aHSqrt);

	// Fonc Loc 

	Fonc_Num aFonc(Rconv(anIm.in_proj()));
	Fonc_Num aS1S2 = Virgule(aFonc, Square(aFonc));
	for (int aK = 0; aK< TheNbIter; aK++)
	{
		aS1S2 = rect_som(aS1S2, TheNbVois) / ElSquare(1 + 2 * TheNbVois);
	}
	Symb_FNum aFS1 = aS1S2.v0();
	Symb_FNum aFS2 = aS1S2.v1() - Square(aFS1);
	Fonc_Num aFLoc = (anIm.in() - aFS1) / sqrt(Max(1.0, aFS2));

	// aFLoc = aFLoc * 20;
	aFLoc = atan(aFLoc) / (PI / 2);


	// Symb_FNum aFonc(aTif.in());

	std::cout << "MaxMin " << aVMax << "\n";

	Fonc_Num aFEg = aH.in()[anIm.in()];
	Fonc_Num aFEgS = aHSqrt.in()[anIm.in()];
	Fonc_Num aFM = anIm.in() * (255.0 / aVMax);


	// Fonc_Num aFRes = (aFEg*aPEg + aFM * aPM + aFEgS*aPSrtEg + aFLoc) / (aPEg + aPM + aPSrtEg);

	Symb_FNum aFMoy = (aFEg*aPEg + aFM * aPM + aFEgS*aPSrtEg) / (aPEg + aPM + aPSrtEg);
	Symb_FNum aFMarge = Min(64, Min(aFMoy, 255 - aFMoy));
	Fonc_Num aFRes = aFMoy + aFMarge * aFLoc;


	/*
	Fonc_Num aFRes = Max(0,Min(255,aFMoy + 60 * aFLoc));
	*/

	Tiff_Im::Create8BFromFonc(aNameOut, aSz, aFRes);



	return EXIT_SUCCESS;
}
#endif