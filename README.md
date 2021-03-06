# Star Clock (North Star and Big Dipper)
The program takes a jpg or bmp file. Converts it into an array of pixels and using a user-given threshold determines the star pixels. The an array of coordinates of middle points of all the stars in the picture is generated. Those middle points are used to determine angle between the stars. For a star to match 2 angels should match the angles from the database - North Star to Reference0 to selected star and North Star to Reference1 to selected stars. For now the reference 0 and 1 stars along with North Star should be in the frame to compute the angles.

Once the program matches all the angles of the stars of Big Dipper, those coordinates are saved. Then those coordinates are used to determine angle of Big Dipper from North Star and that is used to get RAW time.

Raw time is then adjusted using the given date by user to calculate the final time. On March 6th Raw time matches actual time is offset by 2 hours every month.

Once the time is determined, the coordinates that are already stored for Big Dipper stars and North Star are used to draw lines on the picture and return that picture to the user.

The executable file takes 5 arguments. Input file -- Output file -- Month -- Date -- Threshold for stars -- Epsilon (suggested value 0.02).

Epsilon value is the acceptable error while matching the star angles to the database. An error of 0.02 means that computed angle for a star can of off by 0.02 radians compared to database and it would still consider it a match. If you get a false match on your image, try using a smaller Epsilon value.

The pictures used for testing are fairly good quality. There are 2 types of pictures used for test. The first type is pictures taken using the Pro mode on S8 where you can manually adjust the ISO and Aperture settings. These pictures can be used with fairly low threshold for stars as they have minimal background light. Significantly greater number of stars can be extracted from these pictures. The other type is taken using the Auto Camera mode of S8. These pictures have brighter backgrounds so the threshold has to be set around 50 to 100 to extract the needed stars.

yyyymmdd_hhmmss.jpg format is used to name the images.
Images with inverted in the start means that these images were taken with the wrong orientation and are upside down. Even though the time gotten when running these images does not correspond to actual time(is exactly 12h behind), they can be used to demonstrate that if stars were in a different orientation you can still get time. As of now I don't have a way to change their orientation. Most of the softwares just change where pixels are gone be read from(top or bottom), when displaying the pictures, instead of changing the actual orientation of picture.

stb_image library is used to read and write the images. Make sure you include the header files "stb_image_write.h" and "stb_image.h" in your main program.


