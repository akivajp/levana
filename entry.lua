require 'lev.std'

-- View Definitions

frm = lev.gui.frame()
stc = lev.gui.code_edit(frm)

mb = lev.gui.build_menubar {
  {_'&File',
    {_'&New\tCtrl+N', func = 'frm.on_new'},
    {_'&Open...\tCtrl+O', func = 'frm.on_open'},
    {},
    {_'&Save\tCtrl+S', func = 'frm.on_save'},
    {_'Save &As...\tShift+Ctrl+S', func = 'frm.on_save_as'},
  },
  {_'&Edit', name = 'edit',
    {_'&Undo\tCtrl+Z', function() stc:undo() end, id = 'undo'},
    {_'&Redo\tShift+Ctrl+Z', function() stc:redo() end, name = 'redo'},
    {},
    {_'Cu&t\tCtrl+X', function() stc:cut() end},
    {_'&Copy'},
    {_'&Paste'},
    {_'&Delete'},
    {},
    {_'Select &All'},
  },
  {_'&Execute',
    {_'&Execute This Code\tF5', func = 'frm.on_exec'},
    {_'Execute &File...'},
    {_'Execute Fol&der...'},
    {_'Execute &Archive...'},
  }
}

frm.menubar = mb

-- Model & Controls

local function save_select()
  return lev.gui.file_selector {
    _'Choose a file to save',
    wildcard = 'Lev files (*.lev)|*.lev|Lua files (*.lua)|*.lua',
    flag = 'save' }
end

frm.on_close = function(e)
  if changed then
    local msg = string.format('Save changes to document (untitled) before closing?')
    local ans = lev.gui.msgbox {_(msg), style='yes:no:cancel'}
    if ans == true then
      local filename = lev.gui.file_selector {'Choose a file to save', flag='save'}
      if filename:len() > 0 then
        print('Saving...')
        e:skip()
      end
    elseif ans == false then
      e:skip()
    end
  else
    e:skip()
  end
end

frm.on_exec = function()
  local pout = io.popen(lev.fs.exe_path()..' -', 'w')
  pout:write(stc.text)
  pout:flush()
  pout:close()
end

frm.on_new = function()
  if changed then
    local msg = string.format('Save changes to document (untitled) before closing?')
    local ans = lev.gui.msgbox {_(msg), style='yes:no:cancel'}
    if ans == true then
      local filename = lev.gui.file_selector {'Choose a file to save', flag='save'}
      if filename:len() > 0 then
        print('Saving...')
        stc.text = ''
        changed = false
      end
    elseif ans == false then
      stc.text = ''
      changed = false
    end
  else
    stc.text = ''
  end
end

stc.on_char = function(e)
  e:skip()
  changed = true
end

stc.on_idle = function(e)
  if changed then
    frm.title = '*Levana Editor'
  else
    frm.title = 'Levana Editor'
  end
  mb.edit.redo:enable(stc:can_redo())
  mb.edit.undo:enable(stc:can_undo())
end

stc.on_keydown = function(e)
  e:skip()
end


frm:show()
stc.lang = 'lua'
app:autoloop()

