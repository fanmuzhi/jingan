#pragma once
#include "Syn_BravoFingerprintTest.h"


class Ts_BravoSharpness : public Syn_BravoFingerprintTest
{
public:
	Ts_BravoSharpness(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	
	virtual ~Ts_BravoSharpness();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

private:

	void get_sharpness(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessData* pSharpness);

	int find_min(float* pArray, int size);

	int find_max(float* pArray, int size);

	//differential along x and y - > Gx and Gy
	//gradient() will populate Gx and Gy matrices.
	void gradient(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int Gx[MAXROW][MAXCOL], int Gy[MAXROW][MAXCOL], SharpnessData* pSharpness);

	SharpnessData *_pSharpnessData;

};

