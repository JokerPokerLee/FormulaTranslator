echo
echo "********************Start Sompile*********************"
echo
make -s all
if [[ $? == 0 ]]; then
	if [ ! -f ./.compile_acted ]; then
		echo All files are up-to-date.
	else
		rm -f ./.compile_acted
	fi
	echo
	echo "*******************Compile Complete*******************"
	echo
	cd bin 
	./formula_translator
else
	echo
	echo "***********Compile terminated with error(s)***********"
	echo
fi
