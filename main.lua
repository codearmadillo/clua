function clua.start()
    print('start')
end
function clua.update(dt)
    if (clua.keyboard.isKeyDown('a')) then
        print('a is down')
    end
end
function clua.keyboard.keyPressed(key)
    print('released ' .. key);
end
function clua.keyboard.keyReleased(key)
    print('pressed ' .. key);
end