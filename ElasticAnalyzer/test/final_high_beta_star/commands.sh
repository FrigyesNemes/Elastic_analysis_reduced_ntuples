#eos ls /eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/ > files.txt
#grep root files.txt > root_files.txt

#DIAGONAL=LBRT
DIAGONAL=LTRB
for i in `cat root_files.txt`
do
	#echo $i
	#eos ls /eos/totem/data/offline/2018/450GeV/beta11/Totem1/version5/$i
	#echo
	filename2=`echo $i | sed "s/.root//g" | sed "s/\./_/g" `
	filename=/eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/$i
	echo $i
	echo filename  $filename
	echo filename2 $filename2
	echo 
	
	myfilename=cfg_analyzer_template_$filename2"_"$DIAGONAL.py
	echo myfilename $myfilename

	
	cat cfg_analyzer_template.py | sed "s#XXXX#$filename#g" | sed "s#YYYY#$i#g" | sed "s/DIAGONAL/$DIAGONAL/g" > config/$myfilename
	
	#echo "cmsRun config/$myfilename &" >> ntuples/cms_job.sh
done
