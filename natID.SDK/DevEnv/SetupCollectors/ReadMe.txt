To create your own installer or package on Windows, Linux, or macOS, you will need to adjust 
EnvVariables.xml to match your configuration.The recommended approach is to copy the contents of this 
folder outside the natID folder structure. This allows you to adjust all XML files as needed and continue 
using them after a natID update or upgrade.

For example, SuperMario.xml is a configuration file used to generate the Super Mario installer.
Adjust the XML line:
    <Folders dev="~natID.Examples/A_Selected/Snake" executable="$RAMDisk/Out/Snake/Release" out="$RAMDisk/Setup/Snake"/>
where:
    - dev is the location of your source code project
    - executable is the location of your release build (where the executable is located)
    - out is the location where your installer will be created

After copying and adjusting the XML, use the SetupCollector app from natID.Utils/system 
(where system is windows, macOS, or linux):

SetupCollector ~pathToYourSetupsDir/SuperMario.xml -exec=$RAMDisk/Out/SuperMario/Release
