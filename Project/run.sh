rm -f ./.compile_acted
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
	./formula_translator ../input/formula.in
else
	echo
	echo "***********Compile terminated with error(s)***********"
	echo
fi
