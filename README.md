gSpeakXBMC
==========

A voice recognition software built on Pocketsphinx that can interact with the XBMC media center.<br />

This software is made entirely in C++ and is will give you the ability to interact with your XBMC media center with the use of your voice.<br /><br />
It is a hobby project of mine and because of that I don't intend to expand on it any more than I have to for my personal usage but feel free to look at the code if you want to implement something similar yourself.<br />
It uses Pocketsphinx for voice recognition, Festival for speech synthesis and cUrl for sending commands to the XBMC API.

<b>config.cpp</b> - A configuration parser, nothing special.<br /><br />
<b>interpret.cpp</b> - The file that takes a command from Pocketsphinx and translates that into a JSON string that will be sent to the XBMC API, look in this file if you want to know how to use cUrl with the XBMC API.<br /><br />
<b>listen.cpp</b> - This file contains the Pocketsphinx function, it is a modified version of the "pocketsphinx_continuous" application that comes with Pocketsphinx.<br /><br />
<b>speak.cpp</b> - The Festival implementation, look in this file if you want to use the Festival headers and libraries for speech synthesis in C++.<br /><br />