\c adgsdb

COPY t_tasktables FROM stdin;
1	decompressReception	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>DECOMPRESS_RECEPTION</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="3">\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>DECOMPRESS</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/TTDecompress</File_Name>\n\t\t\t\t\t<List_of_Parameters count="0">\n\t\t\t\t\t</List_of_Parameters>\n\t\t\t\t\t<List_of_Inputs count="0">\n                    </List_of_Inputs>\n                    <List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>TASKINPREINVENTORY</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/TaskInPreInventory</File_Name>\n\t\t\t\t\t<List_of_Parameters count="0">\n\t\t\t\t\t</List_of_Parameters>\n\t\t\t\t\t<List_of_Inputs count="0"/>\n\t\t\t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>INVENTORY</Name>\n\t\t\t\t\t<Version>02.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/Inventory</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n\t\t\t\t\t<List_of_Outputs count="0">\n\t\t\t\t\t</List_of_Outputs>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t</List_of_Pools>\n</Task_Table>\n\n	2	Decompress TGZ and archive
2	defaultReception	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>DEFAULT_RECEPTION</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="3">\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>TASKINPREINVENTORY</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/TaskInPreInventory</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n                   \t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>GETORIGINADATE</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/GetOriginDate.sh</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n                   \t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>INVENTORY</Name>\n\t\t\t\t\t<Version>02.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/Inventory</File_Name>\n\t\t\t\t\t<List_of_Parameters count="2">\n\t\t\t\t\t\t<Parameter mandatory="true">\n\t\t\t\t\t\t\t<Name>--logdir</Name>\n\t\t\t\t\t\t\t<Value>.</Value>\n\t\t\t\t\t\t</Parameter>\n\t\t\t\t\t\t<Parameter mandatory="true">\n\t\t\t\t\t\t\t<Name>--relaxFnCheck</Name>\n\t\t\t\t\t\t\t<Value></Value>\n\t\t\t\t\t\t</Parameter>\n\t\t\t\t\t</List_of_Parameters>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n\t\t\t\t\t<List_of_Outputs count="0">\n\t\t\t\t\t</List_of_Outputs>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t</List_of_Pools>\n</Task_Table>\n\n	2	PreInventory and Inventory
3	defaultReceptionZip	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>DEFAULT_RECEPTION_ZIP</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="4">\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>TASKINPREINVENTORY</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/TaskInPreInventory</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n                   \t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>GETORIGINDATE</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/GetOriginDate.sh</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n                   \t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>ZIPSAFEFILE</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/ZipSafeFile.sh</File_Name>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n                   \t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>INVENTORY</Name>\n\t\t\t\t\t<Version>02.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/Inventory</File_Name>\n\t\t\t\t\t<List_of_Parameters count="2">\n\t\t\t\t\t\t<Parameter mandatory="true">\n\t\t\t\t\t\t\t<Name>--logdir</Name>\n\t\t\t\t\t\t\t<Value>.</Value>\n\t\t\t\t\t\t</Parameter>\n\t\t\t\t\t\t<Parameter mandatory="true">\n\t\t\t\t\t\t\t<Name>--relaxFnCheck</Name>\n\t\t\t\t\t\t\t<Value></Value>\n\t\t\t\t\t\t</Parameter>\n\t\t\t\t\t</List_of_Parameters>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n\t\t\t\t\t<List_of_Outputs count="0">\n\t\t\t\t\t</List_of_Outputs>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t</List_of_Pools>\n</Task_Table>\n\n	2	PreInventory Zip and Inventory
4	defaultDelivery	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>DEFAULT_DELIVERY</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="1">\n\t\t<Pool>\n\t\t\t<Detached>false</Detached>\n\t\t\t<Killing_Signal>15</Killing_Signal>\n\t\t\t<List_of_Tasks count="1">\n\t\t\t\t<Task>\n\t\t\t\t\t<Name>PackageDistCurlTask</Name>\n\t\t\t\t\t<Version>01.00</Version>\n\t\t\t\t\t<Critical>true</Critical>\n\t\t\t\t\t<Criticality_Level>1</Criticality_Level>\n\t\t\t\t\t<File_Name>/usr/local/components/ADGS/bin/PackageDistCurlTask</File_Name>\n\t\t\t\t\t<List_of_Parameters count="0">\n\t\t\t\t\t</List_of_Parameters>\n\t\t\t\t\t<List_of_Inputs count="0">\n\t\t\t\t\t</List_of_Inputs>\n\t\t\t\t\t<List_of_Outputs count="0"/>\n\t\t\t\t\t<List_of_Breakpoints count="0"/>\n\t\t\t\t</Task>\n\t\t\t</List_of_Tasks>\n\t\t</Pool>\n\t</List_of_Pools>\n</Task_Table>\n\n	3	default delivery proc chain
5	nopDeliveryCheck	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>NOP_DELIVERY_CHECK</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="0">\n\t</List_of_Pools>\n</Task_Table>\n\n	3	NOP Task Table
6	defaultDistribFormat	<Task_Table xmlns:a="http://www.acsys.it/schemas/IPF" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://www.acsys.it/schemas/IPF TaskTable.xsd">\n\t<Processor_Name>DEFAULT_DISTRIB_FORMAT</Processor_Name>\n\t<Version>1.0</Version>\n\t<Test>No</Test>\n\t<Min_Disk_Space units="MB">1024</Min_Disk_Space>\n\t<Max_Time units="sec">0<!--no time limit for execution-->\n\t</Max_Time>\n\t<Private_Config>\n\t\t<List_of_Cfg_Files count="0">\n\t\t</List_of_Cfg_Files>\n\t\t<Default>0</Default>\n\t</Private_Config>\n\t<List_of_Config_Spaces count="0">\n\t</List_of_Config_Spaces>\n\t<List_of_Pools count="0">\n\t</List_of_Pools>\n</Task_Table>\n\n	3	NOP Task Table
\.
