while getopts x: flag
do
    case "${flag}" in
        x) extract=${OPTARG}
    esac
done

if [ -z "$extract" ]
then
    echo "Try download $1"
    if [ $# -eq 1 ]
    then
        apt-get download $(apt-rdepends $1 | grep -v ^\ )
    elif [ $# -eq 2 ]
    then
        # download and pack into one file
        mkdir dl_pkg_$1
        cd dl_pkg_$1
        apt-get download $(apt-rdepends $1 | grep -v ^\ )
        cd ..
        tar -cf $2 ./dl_pkg_$1
    else
        echo "Util to download deb packages with its dependencices to current directory, needs apt-rdepends to be installed."
        echo "Usage:\n    dl_pkg pakage_name [tar_file_name].\n    dl_pkg -x deb_dir target_dir"
    fi
else
    echo "Extract deb packages"
    # unpack .debs to target dir
    for file in $extract/*.deb
    do
        echo "extract $file to $3"
        dpkg -x $file $3
    done    
fi