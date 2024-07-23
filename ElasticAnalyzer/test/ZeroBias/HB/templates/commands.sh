for i in `cat /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do index=`grep $i ZeroBiasFilesPerRun.txt | awk '{ print $2 }'` ; echo $i $index ; done


for i in `cat /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do index=`grep $i ZeroBiasFilesPerRun.txt | awk '{ print $2 }'` ; let index=index-1; echo $i $index ; cat cfg_analyzer_template_run_XXXX_re_reco_ZeroBias.py | sed "s/XXXX/$i/g" | sed "s/YYYY/$index/g" > config/cfg\_analyzer\_template\_run\_$i\_re\_reco\_ZeroBias.py ; done


for i in `cat /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do cat cfg_analyzer_template_run_XXXX_re_reco_ZeroBias_header.py > config2/cfg\_analyzer\_template\_run\_$i\_re\_reco\_ZeroBias.py ; done
