using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace MT_TestGUI_Demo
{
    public partial class MT_Analysis_GUI : Form
    {
        //===============================================
        // Initilization of GUI
        //===============================================
        public MT_Analysis_GUI()
        {
            InitializeComponent();

            // Disabling test buttons
            //Start_Test_Button.Enabled = false;
            Continue_Test_Button.Enabled = false;
            Continue_Test_Button.Visible = false;
            SelectLogFile_Button.Enabled = false;
            LogFile_Path.Enabled = false;

            // Informing the user
            TestStatus_TextBox.Text = "Select MT Config File";

            // Loading the Bincode
            loadBinCodes();
        }

        //===============================================
        // GUI Variables
        //===============================================
        string fullMTConfigName = "";
        string fullMTLogName = "";

        //===============================================
        // Buttons
        //===============================================

        // Button to select a test config
        private void Select_MT_Config_Button_Click(object sender, EventArgs e)
        {
            if (selectMTConfig())
            {
                TestStatus_TextBox.Text = "Test Ready";
                Start_Test_Button.BackColor = System.Drawing.Color.LightGreen;
            }            
        }

        // Selects a log file
        private void SelectLogFile_Button_Click(object sender, EventArgs e)
        {
            selectLogFile();
        }

        /* Button to Start and Stop
         * If the MT config has not been selected yet, this will also ask the user
         * to select an MT config
         * 
         * If it's own text is "Stop Test" then it will stop the test
        */
        private void Start_Test_Button_Click(object sender, EventArgs e)
        {
            switch (Start_Test_Button.Text.ToUpper())
            {
                case "START TEST":
                    if (!System.IO.File.Exists(fullMTConfigName))
                    {
                        if (!selectMTConfig())
                        {
                            break;
                        }
                    }
                    startTest();
                break;
                case "STOP TEST":

                    break;
                default:
                    break;
            }
        }

        // 
        private void Continue_Test_Button_Click(object sender, EventArgs e)
        {

        }

        //===============================================
        // GUI Update Functions
        //===============================================

        // Function to select and parse MT config file
        private bool selectMTConfig()
        {
            // Creates and sets up an instance of the OpenFileDialog
            OpenFileDialog OFD = new OpenFileDialog();
            OFD.DefaultExt = ".xml";
            OFD.Filter = "MT Config Files (*.xml)|*.xml";
            if (isStringEmpty(fullMTConfigName))
            {   // If the fullMTConfig is empty, use the current root folder
                OFD.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
            }
            else
            {   // If the fullMTConfig has something, use that folder path
                OFD.InitialDirectory = System.IO.Path.GetDirectoryName(fullMTConfigName);
            }


            // Opens the file dialog and checks if the user selected a config file
            if (OFD.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                // Loading config file and checks if it has "TestSeq"
                // Checking for "TestSeq" should tell if this XML is a MT config
                fullMTConfigName = OFD.FileName;
                XmlDocument MTConfigXML = new XmlDocument();
                MTConfigXML.Load(fullMTConfigName);
                XmlNode MTConfigNode = MTConfigXML.SelectSingleNode("TesterConfig");
                if (MTConfigNode.SelectSingleNode("TestSeq") == null)
                {   // If the .xml file does not contain a "TestSeq" node, assume file is not MT config
                    MessageBox.Show(".XML file selected is not a MT Config file");
                    fullMTConfigName = "";
                    return false;
                }

                // Checks if there is a MT Test GUI config file
                // If config exists and contains the correct nodes, use the Subtest_convert file specified in the config,
                //  if not, use default file
                string subtestReadable = "";
                if (System.IO.File.Exists("MTAnalysisConfig.xml"))
                {
                    // Getting "Subtest_convert" file
                    XmlDocument MTAnalysisConfig = new XmlDocument();
                    MTAnalysisConfig.Load("MTAnalysisConfig.xml");
                    if (!(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG") == null) && 
                        !(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("SUBTEST_CONVERT") == null) &&
                        System.IO.File.Exists(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("SUBTEST_CONVERT").InnerText))
                    {
                        subtestReadable = MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("SUBTEST_CONVERT").InnerText;
                    }
                }
                if(isStringEmpty(subtestReadable))
                {
                    subtestReadable = System.IO.Directory.GetCurrentDirectory() + "\\SubTest_Readable.txt";
                }
                
                
                


                // Checks if there is a conversion file and loads it into a dictionary
                // This file changes the subtest name in the MT Config to something easier to read
                // Expected format: "SubTestName : ReadableName", each subtest gets a line
                Dictionary<string, string> convertSubTest = new Dictionary<string, string>();
                if (System.IO.File.Exists(subtestReadable))
                {
                    string[] readLines = System.IO.File.ReadAllLines(subtestReadable);
                    foreach (string item in readLines)
                    {
                        convertSubTest.Add(item.Split(':')[0].ToUpper().Trim(), item.Split(':')[1].Trim());
                    }
                }


                // Select the "TestSeq" node and pull out all the sub-tests 
                MTConfigNode = MTConfigNode.SelectSingleNode("TestSeq");
                ListViewItem[] subTestsFound = new ListViewItem[MTConfigNode.ChildNodes.Count];
                for (int i = 0; i < subTestsFound.Length; i++)
                    subTestsFound[i] = new ListViewItem();
                for (int i = 0; i < subTestsFound.Length; i++)
                {
                    if (convertSubTest.ContainsKey(MTConfigNode.ChildNodes[i].InnerText.ToUpper()))
                        subTestsFound[i].Text = convertSubTest[MTConfigNode.ChildNodes[i].InnerText.ToUpper()];
                    else
                        subTestsFound[i].Text = MTConfigNode.ChildNodes[i].InnerText;

                    subTestsFound[i].SubItems.Add("Ready");
                    subTestsFound[i].SubItems.Add("Loaded");
                }


                // Loading the parsed subtests into the subtest listview (Clears the items first)
                Subtest_Listview.Items.Clear();
                Subtest_Listview.Items.AddRange(subTestsFound);


                // Showing user that config file is loaded and test is ready to run
                Config_File_Selected.Text = System.IO.Path.GetFileName(fullMTConfigName);
                //Start_Test_Button.Enabled = true;
                SelectLogFile_Button.Enabled = true;
                LogFile_Path.Enabled = true;
                return true;
            }
            else
            {
                return false;
            }
        }

        // Function to read the BinCodes file
        private void loadBinCodes()
        {
            if (System.IO.File.Exists("MTAnalysisConfig.xml"))
            {
                // Getting "Subtest_convert" file
                XmlDocument MTAnalysisConfig = new XmlDocument();
                MTAnalysisConfig.Load("MTAnalysisConfig.xml");

                // Getting & Reading "Bincode_Doc" file
                if (!(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG") == null) &&
                    !(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("BINCODE_DOC") == null) &&
                    System.IO.File.Exists(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("BINCODE_DOC").InnerText))
                {
                    string[] BinCode_Read = System.IO.File.ReadAllLines(MTAnalysisConfig.SelectSingleNode("MT_ANALYSIS_CONFIG").SelectSingleNode("BINCODE_DOC").InnerText);
                    int i = 0, j;
                    for (; i < BinCode_Read.Length; i++)
                    {
                        string[] tempSplit = BinCode_Read[i].Split(new char[] { '\t' });
                        if (tempSplit.Length >= 3 && int.TryParse(tempSplit[2], out j))
                        {
                            break;
                        }
                    }

                    j = i;

                    ListViewItem[] binCode = new ListViewItem[BinCode_Read.Length - j];
                    for (int k = 0; k < binCode.Length; k++)
                        binCode[k] = new ListViewItem();
                    for (; i < BinCode_Read.Length; i++)
                    {
                        string[] tempSplit = BinCode_Read[i].Split(new char[] { '\t' });
                        if (tempSplit.Length > 2)
                        {
                            binCode[i - j].Text = tempSplit[2];
                            binCode[i - j].SubItems.Add(tempSplit[0]);
                            binCode[i - j].SubItems.Add(tempSplit[1]);
                        }
                        else if (tempSplit.Length == 2)
                        {
                            binCode[i - j].Text = tempSplit[1];
                            binCode[i - j].SubItems.Add(tempSplit[0]);
                        }
                    }
                    BinCodes_Listview.Items.AddRange(binCode);
                }
            }
        }

        // Function to select a log file to save to
        private void selectLogFile()
        {
            // Creates and sets up an instance of the OpenFileDialog
            SaveFileDialog SFD = new SaveFileDialog();
            SFD.DefaultExt = ".csv";
            SFD.Filter = "LogFile (*.csv)|*.csv";
            if (isStringEmpty(fullMTLogName))
            {   // If the fullMTLogName is empty, use the current root folder
                SFD.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyComputer);
            }
            else
            {   // If the fullMTLogName has something, use that folder path
                SFD.InitialDirectory = System.IO.Path.GetDirectoryName(fullMTLogName);
            }

            // Opens the file dialong and checks if the user selected a file or not
            if (SFD.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                // Saving the user selected config file
                fullMTLogName = System.IO.Path.GetDirectoryName(SFD.FileName);
                LogFile_Path.Text = System.IO.Path.GetFileName(SFD.FileName);
            }
        }

        // 

        //===============================================
        // Test Functions
        //===============================================

        // Function to start the test
        private bool startTest()
        {
            Start_Test_Button.BackColor = System.Drawing.Color.MistyRose;
            Start_Test_Button.Text = "Stop Test";



            return false;
        }

        // Function to poll and update the GUI
 

        //===============================================
        // Other Functions
        //===============================================

        


        // Function to check if the string is empty
        private bool isStringEmpty(string stringToCheck)
        {
            return string.IsNullOrEmpty(stringToCheck) || string.IsNullOrWhiteSpace(stringToCheck);
        }
    }
}
