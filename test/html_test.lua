require 'lev.std'

frm = lev.gui.frame {}
sizer = lev.gui.vsizer()
frm.sizer = sizer
txt = lev.gui.textbox {p = frm}
sizer:add(txt,0)
html = lev.gui.htmlview(frm)
sizer:add(html,1)
--sizer:fit(frm)
--frm:fit()
frm:show()

txt.on_keydown = function(e)
  print("id  = " .. e.id)
  print("key = " .. e.key)
  if e.keystr == "return" then
    web = lev.net.url.connect('http://www.google.com')
--    local url = 'http://info.babylon.com/onlinebox.cgi?rt=ol&tid=pop&cid=CD1&tl=ja&term=' .. txt.value
--    html:loadpage(url)
--    local sock = lev.net.url.connect(url)
--    local body = sock.data
--    local defs = ''
--    for w in body:gmatch('<div class="definition" dir="">.-</div>') do
--      defs = defs .. w
--    end
--    for w in body:gmatch('<div style=%b"" class="definitionCross" dir="">.-</div>') do
--      defs = defs .. w
--    end
--    html.page = defs
  else
    e:skip()
  end
end

--html:loadpage('http://www.boost.org')
html:loadpage('http://www.kde.org')
app:autoloop(frm)

