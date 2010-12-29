require 'lev/all'

function init()
  frm = lev.frame()
  local mb = lev.menubar()
  local file = lev.menu()
  file:append {str = '&Exit', func = function() frm:close() end}
  mb:append(file, '&File')

  local tray = lev.systray()
  local sysmenu = lev.menu()
  sysmenu:append {str = '&Test', func = function() frm:close() end}
  tray:setmenu(sysmenu)

  frm = lev.frame()
  frm:setmenubar(mb)
  html = levana.htmlview(frm, 640, 480)
  frm:fit()
--  html:loadpage("http://www.google.co.jp/")
  frm:show()
  frm.status = 'OK'
end

init()
app:setonkeydown(function(evt) app:msgbox(tostring(evt:getkey()) .. ' keypress') end)

while frm:exists() do
  app:yield()
end

