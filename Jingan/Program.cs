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
            OFilmAdcBaseLineInfo abi = new OFilmAdcBaseLineInfo();
            abi.m_nVdd = 1800;
            abi.m_nVio = 1800;
            abi.m_nVled = 3300;
            abi.m_nVddh = 3300;
            OFilmDeviceManager dm = new OFilmDeviceManager();
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

            string cfgPath = "C:\\Users\\bqiling\\Google Drive\\Metallica 57K0\\Huangpu Version\\Metallica_57K0_OFilm_Grey.xml";
            OFilmSite site = new OFilmSite(1, deviceSN, cfgPath, abi);
            UInt32 rc = site.Open();
            Console.WriteLine(rc);

            List<String> testSteps = site.GetTestStepList();
            OFilmTestResult tR = new OFilmTestResult();
            foreach (String step in testSteps)
            {
                Console.WriteLine(step);
                if(step == "AcqImgFinger")
                {
                    Console.WriteLine("please put stimulus on and return");
                    Console.ReadKey();
                    Console.WriteLine("acqire finger image ....");
                }
                UInt32 error = site.ExecuteTestStep(step);
                if (error != 0)
                {
                    Console.WriteLine(String.Format("{0:X}", error));
                    break;
                }

                error = site.GetTestResult(ref tR);
                if (error != 0)
                {
                    Console.WriteLine(String.Format("{0:X}", error));
                    break;
                }
                Console.WriteLine(tR.StepResult.Count);
            }
            site.Close();

            Console.ReadKey();
        }
    }
}
