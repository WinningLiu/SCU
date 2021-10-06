echo Executing $0
echo $(/bin/ls | wc -l) files
wc -l $(/bin/ls)
echo "HOME="$HOME
echo "USER="$USER
echo "PATH="$PATH
echo "PWD="$PWD
echo "\$\$"=$$
user=`whoami`
numusers=`who | wc -l`
echo "Hi $user! There are $numusers users logged on."
if [ $user = "dliu1" ]
then
    echo "Now you can proceed!"
else
    echo "Check who logged in!" #print and exit if user is not dliu1
    exit 1
fi

#Calculates area of rectangle
response="Yes"
while [ $response != "No" ] #repeat until user says No
do 
    echo "Enter height of rectangle: "
    read height 
    echo "Enter width of rectangle: "
    read width
    area=`expr $height \* $width` #calculaate height * width
    echo "The area of the rectangle is $area"

    echo "Would you like to repeat for another rectangle [Yes/No]?"
    read response
done

#Calculates area of circle
response1="Yes"
while [ $response1 != "No" ]
do
    echo "Enter radius of circle: "
    read radius
    area=`expr "$(($radius**2)) * 3.14159" | bc` #Calculate r**2 * pi
    echo "The area of the circle is $area"

    echo "Would you like to repeat for another circle [Yes/No]?"
    read response1
done