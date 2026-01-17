# Cricket07FileLoader
Load files from through real filename instead of hashed filename and increases limit used for game resources.
<ul>
  <li>By default in Cricket 07, one can only load game files with name which are md5-hashes. This plugin changes it and supports loading of files through their <a href="https://github.com/retrobone/Cricket07Redir">real names</a>.</li>
<br>
<ul>
  <li>
  For eg - To use another overlay you needed to paste <b>7ac69a95d810236a72e073135195d310.big</b> and <b>557e1a2a7bd98f3680af6f879c16d401.fsh</b> in the main game directory but through this plugin you can use it from <b>    <i>frontend/ig_overlay.big</i></b> and <b><i>frontend/textures/ig_ingame.fsh</i></b> respectively.
  </li>
</ul>
<br>
    <li>Load bats 100-256 through normal name (bt100.fsh instead of bt:1.fsh), as by default game appended ASCII characters to bat id > 100, this plugin fixes that - game sees in ASCII way, user sees the normal way</li>
<br>
  <li>By default the game memory limit used for resources is <b>100663296 bytes</b> i.e. <b>96 MB</b>. This plugin increase that memory limit.</li>
</ul>

#  Installation
<ul>
  <li>Download Ultimate-ASI-Loader(ddraw.dll) from <a href="https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases">here</a> and extract where your game is </li>
  <li><a href="https://github.com.retrobone/Cricket07FileLoader/releases">Download</a> the plugin and extract it to your main game directory </li>
</ul>
