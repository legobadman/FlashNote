
export PATH=/home/luzh/Qt/install_release_shared/bin:$PATH
export LD_LIBRARY_PATH=/home/luzh/Qt/install_release_shared/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=/home/luzh/Qt/install_release_shared/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=/home/luzh/Qt/install_release_shared/qml:$QML2_IMPORT_PATH

rm -rf source output
#sudo apt-get remove flashnote

mkdir -p source
mkdir -p output
cd source
mkdir -p DEBIAN
mkdir -p opt
cd opt
mkdir -p flashnote
cd flashnote

cp /home/luzh/FlashNote/Release/bin/* .
#copy desktop icon
cp /home/luzh/FlashNote/Code/guimain/res/icons/flash.png .
cp -rf /home/luzh/FlashNote/Code/setup/startup .

linuxdeployqt flashnote -appimage

rm default.desktop

output=flashnote.desktop

echo "[Desktop Entry]" | tee $output
echo "Type=Application"	| tee -a $output
echo "Name=FlashNote" | tee -a $output
echo "Exec=/opt/flashnote/flashnote" | tee -a $output
echo "Icon=/opt/flashnote/flash.png" | tee -a $output
echo "Comment=flash note everything" | tee -a $output
echo "Terminal=false" | tee -a $output

cd ../../DEBIAN

output=control

echo "Package: flashnote"	| tee $output
echo "Version: 0.1.0"		| tee -a $output
echo "Section: utils"		| tee -a $output
echo "Priority: optional"	| tee -a $output
echo "Architecture: amd64"	| tee -a $output
echo "Depends:"			| tee -a $output
echo "Installed-Size: 512"	| tee -a $output
echo "Maintainer: 626332185@qq.com"| tee -a $output
echo "Description: test app"	| tee -a $output

output=postinst

echo "#!/bin/sh" | tee $output
echo "chmod 777 /opt/flashnote/flashnote.desktop" | tee -a $output
echo "cp /opt/flashnote/flashnote.desktop /usr/share/applications/" | tee -a $output
echo "cp -rf /opt/flashnote/startup ~/.local/share/flashnote" | tee -a $output
#echo "cp /opt/flashnote/flashnote.desktop ~/桌面/" | tee -a $output
chmod 0755 postinst

output=postrm

echo "#!/bin/sh" | tee $output
echo "rm /usr/share/applications/flashnote.desktop || true" | tee -a $output
#echo "rm -rf ~/.local/share/flashnote" | tee -a $output
#echo "rm ~/桌面/flashnote.desktop || true" | tee -a $output
chmod 0755 postrm

#to source dir
cd ..
dpkg -b . ../output/flashnote_0.1.0.deb
