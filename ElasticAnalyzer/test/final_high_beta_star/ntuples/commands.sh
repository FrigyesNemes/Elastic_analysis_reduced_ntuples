#for i in `cat ../root_files.txt | sed "s/.root//g" | sed "s/\./_/g"` ; do ls "../config/cfg_analyzer_template_"$i"_LTRB.py"  ; done
for i in `cat ../root_files.txt | sed "s/.root//g" | sed "s/\./_/g"` ; do echo "cmsRun " "../config/cfg_analyzer_template_"$i"_LTRB.py &> /dev/null & "  ; done
awk '1;!(NR%4){print "wait";}' cms_job.shtest
eos ls /eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/

for i in `cat info/runs.txt` ; do echo $i ;  eos ls /eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/ | grep root | grep $i ; echo ; done

