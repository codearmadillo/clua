clua.start = function()
    clua.window.setWidth(1024)
    clua.window.setHeight(768)
    clua.window.setTitle('My new game')
end

clua.update = function()
    print("update from lua")
end