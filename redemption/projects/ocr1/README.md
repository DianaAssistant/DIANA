# OCR

- How to learn on a (new) character set?
Character definitions are in learn.ok as .pbm image files
The associated .txt file corresponds to the text in the image.
Each character or set of characters is separated by a space.
Note: '\' is an escape character, double it to take it into account.

- What is the procedure for generating the character recognition module?
$ `make`
this command generates **. hxx ** files and the `classifiers` and` fonts` folders.
The `{, classifiers /} *. Hxx` files are used by the OCR.

/! \ ** Don't forget to modify ** `whitespace_width` ** when a font is added **.

In case of errors with learning, display_learning allows you to see the extracted characters.

- How to start character recognition on a still image?
Use the extract_text program. This only supports ppm extensions.
Note: There are programs like bmptoppm or pngtopnm in the package manager
(`pngtopnm file.png> file.pbm`).



# BUGS

Some characters are recognized as multiple characters.
(for example `ï` in Tahoma is recognized as` .i.`).
This is a learning "bug" and the classification of such a character leads to an error.

To correct the problem a file `font_name.repl` must be created in learn.ok.
This contains the replacement characters (c ++ code):

`{" sequence to find "," replacement sequence "},`



# Win 2012 and smoothing

`tools/ocr/img_win2012_smoothing_to_monochrome input_image output_image`
  transforms a standard windows2012 title bar image to a black and white image

`tools/ocr/img_win2012_smoothing_to_monochrome_d input_images`
  takes several files as parameters and calls the previous script with an extension
  output in .monochrome + in .pbm + a .txt file containing the name of the file (it
  the file must be named by the letters in the image with a separator space
  between each).
  Note that it must be in the images folder because the full path is used
  as text in .txt file

(Note: the colors in rdp and vnc are different, to have the complete list of
colors constituting an image: `identify -verbose -unique mon_image`.)

OCR questions (OBSOLETE)
-----------------------

- What is Milena?

Milena is a generic image processing library developed by LRDE
(research laboratory dependent on Epita). Milena is part of the project
Olena, and that's also the name of Olena's core libraries.

- What are the codes delivered by Epita specific to our project?

The delivery of epita for redemption includes:
- A Makefile -> to merge in the redemption Jamroot
- README and INSTALL files integrated into OCR.txt (this file) and deleted.

- character definition sets: learn.ok folder

- the following python source files:

cart.py -> used by classifier
gen_classifier.py -> create the include classifier.hxx used by classfier.hh
 gen_classifier uses the content of features.txt, itself built from
 from the contents of the learn folder using the learning program.

- the following C ++ sources

classification.hh
classifier.hh
extract_bars.hh -> extract title bars from an image
extract_text.cc
extract_text.hh -> extract the text contained in the title bars
learning.cc -> uses the content of learn (pbm image and character codes) to
            create the features.txt file used by gen_classifier.py


- What are the generic Milena codes used?

The OCR uses the version of milena contained in the milena file, that is to say
things close [differences?] the same as the contents of the milena folder
by olena-1.0. The olena-1.0 archive is kept on file as a
reference, but not directly used during the compilation phase.

- Where are the character set definition files?

- How to learn on a (new) character set?

Character definitions are in learn (link to learn.ok) in the form
.pbm image files
The associated .txt file corresponds to the text in the image.
Each character or set of characters is separated by a space.
Note: '\' is an escape character, double it to take it into account.

- What is the procedure for generating the character recognition module?
make learning

- How to compile Redemption with OCR support?

- How to start character recognition on a still image?
see "make test".

- How to call character recognition from a buffer?



ORIGINAL README (OBSOLETE)
--------------------------

Orders:
+ make all: compilation of the program and training.
+ make learning: learning only from files in
"learn"
+ make test: launch of the test procedure. All screens are