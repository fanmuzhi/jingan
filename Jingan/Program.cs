using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ManagedTestEngine;

namespace Jingan
{
    class Program
    {
        static void Main(string[] args)
        {
            UInt32 deviceSN = 0;
            SYNAAdcBaseLineInfo abi = new SYNAAdcBaseLineInfo();
            abi.m_nVdd = 1800;
            abi.m_nVio = 1800;
            abi.m_nVled = 3300;
            abi.m_nVddh = 3300;
            SYNADeviceManager dm = new SYNADeviceManager();
            dm.Open();
            List<UInt32> SNList = dm.GetSNList();
            if (SNList.Count != 0)
            {
                dm.UpdateFW();
                foreach (UInt32 sn in SNList)
                {
                    Console.WriteLine(sn.ToString());
                    abi.m_arAdcBaseLines = dm.UpdateADCOffsets(sn, abi.m_nVdd, abi.m_nVio, abi.m_nVled, abi.m_nVddh);
                    foreach (UInt32 adc in abi.m_arAdcBaseLines)
                    {
                        Console.Write(adc.ToString() + ", ");
                        Console.WriteLine("");
                    }
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

            string cfgPath = "C:\\Users\\bqiling\\Google Drive\\Metallica 57K0\\Huangpu Version\\Metallica_57K0_SYNA_Grey.xml";
            SYNASite site = new SYNASite(1, deviceSN, cfgPath, abi);
            UInt32 rc = site.Open();
            Console.WriteLine(rc);

            List<String> testSteps = site.GetTestStepList();
            SYNATestResult tR = new SYNATestResult();
            foreach (String step in testSteps)
            {
                //display test step name
                Console.WriteLine(step);

                //if waiting for fake finger
                if(step == "AcqImgFinger")
                {
                    Console.WriteLine("please put stimulus on");
                    Console.ReadKey();
                    Console.WriteLine("acqire finger image ....");
                }
                
                //execute test step
                UInt32 error = site.ExecuteTestStep(step);
                if (error != 0)
                {
                    Console.WriteLine(String.Format("{0:X}", error));
                    break;
                }

                //get test result
                error = site.GetTestResult(ref tR);
                if (error != 0)
                {
                    Console.WriteLine(String.Format("{0:X}", error));
                    break;
                }
                foreach(KeyValuePair<string, string> kv in tR.StepResult)
                {
                    if (kv.Key == step)
                    {
                        //display test result of each test step
                        Console.WriteLine("............................" + kv.Value);
                    }
                }
            }
            //display bin codes and SNR value
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

            site.Close();

            Console.ReadKey();
        }
    }
}
