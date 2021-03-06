#!/bin/sh

###############################################################################
#                                                                             #
# checkout.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.2.9                                                              #
# Date   : 17.11.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/avr_robolib/                                    #
###############################################################################

NAME_THIS="robolib"

###############################################################################
PATH_THIS="${NAME_THIS}/"
NAME_GIT_THIS="avr_${NAME_THIS}"

URL_GIT_BASE="https://github.com/RoboAG/"
URL_GIT_THIS="${URL_GIT_BASE}${NAME_GIT_THIS}.git"

NAME_CHECKOUT_SCRIPT="checkout.sh"

###############################################################################
echo "The project"
echo "  \"${NAME_THIS}\""
echo "will be checked out completely."
echo ""


echo ""
echo "### checking out the project"
if [ -d "${PATH_THIS}" ]; then
    echo "This project already exists!"
    return
    exit
fi
git clone "${URL_GIT_THIS}" "${PATH_THIS}"


echo ""
echo "### automatically sourcing this project"
./${PATH_THIS}scripts/setup_bashrc.sh $1


echo ""
echo "### installing needed packages"
echo "Do you want to install needed packages ? (No/yes)"
echo -n "(This can also be done later by invocing "
echo    "\"make install_prerequisites\")"
if [ "$1" != "-y" ] && [ "$1" != "--yes" ]; then
    read answer
else
    echo "<auto answer \"yes\">"
    answer="yes"
fi
if [ "$answer" != "y" ] && [ "$answer" != "Y" ] && \
  [ "$answer" != "yes" ]; then

    echo "skipped"
else

    bash -c "cd ${PATH_THIS} && make install_prerequisites"
fi


echo ""
echo "### downloading additional files"
echo "Do you want to download additional binaries ? (No/yes)"
echo -n "(This can also be done later by invocing "
echo    "\"make download_additionals\")"
if [ "$1" != "-y" ] && [ "$1" != "--yes" ]; then
    read answer
else
    echo "<auto answer \"yes\">"
    answer="yes"
fi
if [ "$answer" != "y" ] && [ "$answer" != "Y" ] && \
  [ "$answer" != "yes" ]; then

    echo "skipped"
else

    bash -c "cd ${PATH_THIS} && make download_additionals"
fi


if [ $? -ne 0 ]; then
    echo "### There have been errors! ###"
    return -1
    exit -1
else
    echo ""
    echo "### deleting this script"
    rm "${NAME_CHECKOUT_SCRIPT}"

    echo "all done :-)"
fi
