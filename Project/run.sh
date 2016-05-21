echo
echo "********************Start Sompile*********************"
echo
res=$(make all)
if [[ $? == 0 ]]; then
	if [[ $res == *"Nothing to be done"* ]]; then
		echo "All files are up to date."
	else
		echo $res
	fi
	echo
	echo "*******************Compile Complete*******************"
	echo
	cd bin 
	./formula_translator
else
	echo $res
	echo
	echo "***********Compile terminated with error(s)***********"
	echo
fi
