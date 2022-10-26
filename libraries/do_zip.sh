#/bin/bash
rm *.zip
for d in *
do
	if [ -d $d ]
	then
		echo ""
		echo "Creo $d.zip"
		zip -r $d.zip $d
	fi
done
