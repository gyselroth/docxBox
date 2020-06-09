docxBox Changelog
=================

V0.0.5 - Not finished yet
-------------------------
* Add batch sequence processing
* Colorize side-by-side DOCX comparison (ls) to indicate additions/removals
* Add replace text by run with text
* Add replace text by paragraph containing run with text
* Extend rendering of generic content types: render optional prefix and postfix 

V0.0.4 - 2020-05-26
-------------------
* Extend list-fields (lsd): Search also within headers, footnotes, endnotes
* Add colorized unified diff, add headline to side-by-side diff  
* Add handler for all docxBox error messages 
  (prefixes all error-messages, creates central place for logging)  
* Add replace text by ordered and unordered list 
* Add replace text by heading 1, 2, 3 
* Add replace text by image
* Add cat command: output content of file from within DOCX 
* Render help in two columns, output also version number 
* Side-by-side DOCX compare now detects different content (of files w/ identical attributes)
* Side-by-side DOCX compare now marks files present only in one of the documents 

V0.0.3 - 2020-04-29
-------------------
* Add set generic field value - set preview text (sfv)
* Add set merge field value - transform field into text (sfv)
* Ensure re-zipped DOCX is rated valid when opened in MS word
* Add DOCX check before attempting to extract: must be a ZIP archive
* Add list files containing search-string (lsl)
* Add execute user-defined command on contained files (cmd)
* Add side-by-side comparison of file from two DOCX archives (diff)
* Add side-by-side comparison of two DOCX documents (ls)
* Add Compress XMLs and zip (zpc)
* Rename commands: rem -> rmt, zip -> zp
* Add unzip and indent XML (uzi)
* Improve list (ls): add optional wildcard filter
* Improve list images (lsi): output file attributes

V0.0.2 - 2020-04-22
-------------------
* Add remove content between text (rem)
* Add replace text by table (rpt w/ JSON)

V0.0.1 - 2020-04-20
-------------------
* Add replace text by random lorem ipsum (lorem)
* Add replace images of same kind (rpi)
* Add replace text by text (rpt)
* Add output plaintext (txt)
* Add list images (lsi)
* Add list fields (lsd)
* Add list fonts (lsf)
* Add list meta data (lsm)
* Add list contents (ls)
* Add zip (zip)
* Add unzip and unzip media (uz, uzm)
* Implement application base
