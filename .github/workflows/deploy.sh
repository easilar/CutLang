#!/bin/bash

cd $HOME/work/CutLang/CutLang/runs
wget http://www.hepforge.org/archive/cutlang/cms_opendata_ttbar.root
wget http://www.hepforge.org/archive/cutlang/atla_opendata_had_ttbar.root
wget -O delphes_events_ttbar.root https://docs.google.com/uc?export=download\&id=1P8Pv2hmV4QcMfNWmQTsuAkqIYcEzsuxt

for i in $(ls |grep .adl); do
        echo "Testing $i:"
        echo "With CMSOD"
        echo "./CLA.sh ./cms_opendata_ttbar.root CMSOD -i $i -e 10000"
        ./CLA.sh ./cms_opendata_ttbar.root CMSOD -i $i -e 10000
        echo "With ATLASOD"
        echo "./CLA.sh ./atla_opendata_had_ttbar.root ATLASOD -i $i -e 10000"
        ./CLA.sh ./atla_opendata_had_ttbar.root ATLASOD -i $i -e 10000
        echo "With DELPHES"
        echo "./CLA.sh ./delphes_events_ttbar.root DELPHES -i $i -e 10000"
        ./CLA.sh ./delphes_events_ttbar.root DELPHES -i $i -e 10000
done
