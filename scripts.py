class Quad(object):
    def __init__(self, center, bottom_left, top_right, color):
        self.center = center 
        self.bottom_left = bottom_left
        self.top_right = top_right
        self.color = color


def create_quads():
    quad1 = Quad(center=(25, 75),
                 bottom_left=(10, 100),
                 top_right=(40, 60),
                 color=(0, 1, 0))
    quad2 = Quad(center=(300, 300),
                 bottom_left=(200, 350),
                 top_right=(400, 250),
                 color=(0, 0, 1))

    return [quad1, quad2]
