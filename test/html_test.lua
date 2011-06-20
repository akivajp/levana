require 'lev.std'

frm = lev.gui.frame.create {}
sizer = lev.gui.vsizer.create()
frm.sizer = sizer
txt = lev.gui.textbox.create {p = frm}
sizer:add(txt,0)
html = lev.gui.htmlview.create (frm)
sizer:add(html,1)
--sizer:fit(frm)
--frm:fit()
frm:show()

txt.onkeydown = function(e)
  print("id  = " .. e.id)
  print("key = " .. e.key)
  if e.key == "RETURN" then
    local url = 'http://info.babylon.com/onlinebox.cgi?rt=ol&tid=pop&cid=CD1&tl=ja&term=' .. txt.value
    local sock = lev.net.url.connect(url)
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

app:autoloop(frm)

