import pygame

pygame.init()
joystick_count = pygame.joystick.get_count()

if joystick_count > 0:
	joystick = pygame.joystick.Joystick(0)
	joystick.init()
	print(f"Controller Name: {joystick.get_name()}")

	running = True
	while running:
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				running = False
			elif event.type == pygame.JOYBUTTONDOWN:
				print(f"Button {event.button} pressed")
			elif event.type == pygame.JOYAXISMOTION:
				print(f"Axis {event.axis} moved to {event.value}")
			elif event.type == pygame.JOYHATMOTION:
				print(f"Hat {event.hat} moved to {event.value}")
else:
	print("No joystick found.")

pygame.quit()
