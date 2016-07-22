using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using ManagedTestEngine;

namespace Jingan
{
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine(DllVersion.GetDllVersion());

            UInt32 deviceSN = 0;
            SynapticsAdcBaseLineInfo abi = new SynapticsAdcBaseLineInfo();
            abi.m_nVdd = 1800;
            abi.m_nVio = 1800;
            abi.m_nVled = 3300;
            abi.m_nVddh = 3300;
            SynapticsDeviceManager dm = new SynapticsDeviceManager();
            dm.Open();
            List<UInt32> SNList = dm.GetSNList();
            if (SNList.Count != 0)
            {
                Console.WriteLine("please remove the sensor...");
                Console.ReadKey();
                dm.UpdateFW();
                foreach (UInt32 sn in SNList)
                {
                    Console.WriteLine(sn.ToString());
                    abi.m_arAdcBaseLines = dm.UpdateADCOffsets(sn, abi.m_nVdd, abi.m_nVio, abi.m_nVled, abi.m_nVddh);
                    string sadcBaseline = "";
                    foreach (UInt32 adc in abi.m_arAdcBaseLines)
                    {
                        sadcBaseline += adc.ToString() + ", ";
                    }
                    Console.WriteLine("ADC Baseline: " + sadcBaseline);
                    dm.SetLED(sn);
                }
                deviceSN = SNList[0];
            }
            else
            {
                Console.WriteLine("No device found.");
                Console.ReadKey();
                return;
            }

            string cfgPath = @"D:\projects\MTconfig\Metallica_109A\FM192_182_Huawei_109A_109C_Series\release\(580-006033-01r05)_OFilm_109A_Huangpu.xml";
            Console.WriteLine("please mount the sensor...");
            Console.ReadKey();

            SynapticsSite site = null;
            try
            {
                site = new SynapticsSite(1, deviceSN, cfgPath, abi);
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
                Console.ReadKey();
                return;
            }
            UInt32 rc = site.Open();
            Console.WriteLine(rc);

            List<String> testSteps = site.GetTestStepList();
            SynapticsTestResult tR = new SynapticsTestResult();

            Stopwatch timer = new Stopwatch();
            foreach (String step in testSteps)
            {
                //display test step name
                Console.WriteLine(step);

                //if waiting for fake finger
                if (step == "WaitStimulus")
                //if(step == "AcqImgFinger")
                {
                    Console.WriteLine("please put stimulus on");
                    Console.ReadKey();
                    Console.WriteLine("acqire finger image ....");
                }
                else
                {
                    timer.Start();
                    //execute test step
                    UInt32 error = site.ExecuteTestStep(step);
                    if (error != 0)
                    {
                        Console.WriteLine(String.Format("{0:X}", error));
                        break;
                    }

                    timer.Stop();
                    Console.WriteLine("............................Time elapsed: {0} ms", timer.ElapsedMilliseconds);
                    timer.Reset();

                    //get test result
                    error = site.GetTestResult(ref tR);
                    if (error != 0)
                    {
                        Console.WriteLine(String.Format("{0:X}", error));
                        break;
                    }
                    foreach (KeyValuePair<string, string> kv in tR.StepResult) // <Key=TestStep, value=Pass/Fail> Pair
                    {
                        if (kv.Key == step)
                        {
                            //display test result of each test step
                            Console.WriteLine("............................" + kv.Value);
                        }
                    }
                }
            }

            Console.WriteLine("SerialNumber:" + tR.SensorSerialNumber);

            //display bin codes and SNR value
            Console.WriteLine("SensorSerialNumber: " + tR.SensorSerialNumber.ToString());

            Console.WriteLine("SNR: " + tR.SNR.ToString());

            string sBincodes = "";
            foreach (string bincode in tR.BinCodes)
            {
                sBincodes += bincode + ", ";
            }
            if (tR.BinCodes.Count == 0)
            {
                sBincodes += 1;
            }
            Console.WriteLine("Bin Codes: " + sBincodes);



            //display image
            //Console.WriteLine("No finger image:");
            byte NumRow = tR.ImageNoFingerRow;
            byte NumCol = tR.ImageNoFingerCol;
            for (byte col = 0; col < NumCol; col++)
            {
                string rowImage = "";
                for (byte row = 0; row < NumRow; row++)
                {
                    int index = row + (NumRow * col);
                    rowImage += tR.ImageNoFinger[index].ToString() + ", ";
                }
                //Console.WriteLine(rowImage);
            } 
            //Console.WriteLine("finger image:");
            NumRow = tR.ImageFingerRow;
            NumCol = tR.ImageFingerCol;
            for (byte col = 0; col < NumCol; col++)
            {
                string rowImage = "";
                for (byte row = 0; row < NumRow; row++)
                {
                    int index = row + (NumRow * col);
                    rowImage += tR.ImageFinger[index].ToString() + ", ";
                }
                //Console.WriteLine(rowImage);
            } 

            //write log
            site.WriteLog("D:\\logfiles", "");//test.csv

            site.Close();

            Console.ReadKey();
        }
    }
}
