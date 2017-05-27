rm -f ../*.rpm
rm -r -f rpms/*
cp -f ../mcinit root/usr/local/bin/mcinit
togo build package
cp rpms/* ..
