import pygame
import numpy as np




pygame.init()

#define colours
BLACK = (0,0,0)
WHITE = (255,255,255)
RED = (255,0,0)
GREEN = (0,255,0)
BLUE = (0,0,255)
YELLOW = (255,255,0)
ORANGE = (255,165,0)

#set up screen
WIDTH = 600
HEIGHT = 500
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Gravity')
clock = pygame.time.Clock()

#define sprites

class Body(object):
    def __init__(self,m,r,x,v):
        self.m = m
        self.r = r
        self.x = x
        self.v = v

#define simulation intial conditions
bodies_no = 60
T = 10000.
dt = 0.01
SPEED = 1000

G = 6.61e-11
t = 0
m = np.linspace(1e+16,1e+16,bodies_no)
r = np.linspace(3.,3.,bodies_no)
x = []
v = []
for i in range(bodies_no):
    x.append(np.array([np.random.rand()*WIDTH, np.random.rand()*HEIGHT]))
    v.append(np.array([(np.random.rand()-0.5)*SPEED, (np.random.rand()-0.5)*SPEED]))
bodies = []
for i in range(bodies_no):
    bodies.append(Body(m[i],r[i],x[i],v[i]))
    
end = False
counter = 0
while t<T and not end:
    #EVENT HANDLING
    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            end = True
        elif event.type==pygame.KEYDOWN:
            if event.key==pygame.K_x:
                end = True
    
    
    #GAME LOGIC
    #get gravitational forces
    for body in bodies: #effects on each body    
        a = np.array([0.,0.])
        for effect in bodies: #effects from each body
            if body!=effect:
                if effect.x[0] > body.x[0]:
                    theta = np.arctan((effect.x[1]-body.x[1])/(effect.x[0]-body.x[0])) #theta is the direction angle of body to effect vector
                elif effect.x[0] < body.x[0]:
                    theta = np.arctan((effect.x[1]-body.x[1])/(effect.x[0]-body.x[0]))+np.pi
                else:
                    if effect.x[1] > body.x[1]:
                        theta = np.pi/2
                    else:
                        theta = -np.pi/2
                dist = ((effect.x[0]-body.x[0])**2 + (effect.x[1]-body.x[1])**2)**0.5 #distance between two bodies
                
                if dist <= body.r+effect.r:
                    print(('Collision at time {0:.2f} at coordinates x = {1:.2f}, and y = {2:.2f}'.format(t, list((body.x*body.r+effect.x*effect.r)/(body.r+effect.r))[0], \
                    list((body.x*body.r+effect.x*effect.r)/(body.r+effect.r))[1]))) #point of collision: radius weighted mean of positions
                    bodies.remove(effect)
                    body.x = (body.x*body.m+effect.x*effect.m)/(body.m+effect.m) #mass weighted mean of positions
                    body.v = (body.v*body.m+effect.v*effect.v)/(body.m+effect.m) #mass weighted mean of velocities
                    body.m = body.m+effect.m
                    body.r = (body.r**3+effect.r**3)**0.3333
                    #should delete effect object
                    continue
                    
                amag = G*effect.m/dist**2 #magnitude of acceleration
                a[0] += amag*np.cos(theta)
                a[1] += amag*np.sin(theta)
            
        #solve equations of motion
        body.x += body.v*dt + a*dt*dt/2.
        body.v += a*dt
        t += dt
        
        #bounce back from the walls
        if body.x[0] < 0 or body.x[0] > WIDTH:
            body.v[0] = -body.v[0]
        if body.x[1] < 0 or body.x[1] > HEIGHT:
            body.v[1] = -body.v[1]
        
        
    #GRAPHICS
    screen.fill(BLACK)
    for body in bodies:
        pygame.draw.circle(screen, WHITE, (int(body.x[0]),int(body.x[1])), int(body.r))
        
    #save image
    counter +=1
#    filename = 'out'+str(counter)+'.png'
#    pygame.image.save(screen, filename)
        
    pygame.display.flip()
    clock.tick(60)
            
      
pygame.quit()
    


"""questions:
    
would it be better to store bodies in a Bodies class than a Python list?

to do:
    implement new type of collisions using clustering
    implement user directed body
"""