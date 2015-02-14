Hallo Martin :D

Hier nun endlich mein Programm. Leider hat es durch die Texturen ein paar Performance Probeleme
beim Drop and Drag der Punkte und der Slider. Du kannst die Texturen auch auskommentieren oder den Ordner
mit den Texturen verschieben, damit diese nicht angezeigt werden. (Will damit nur sagen, dass es kein
Implementierungsfeler im MousePosCallback ist, dass das Programm beim Drag 'n' Drop etwas hinterherhinkt..)
Die Schrift in der Textur hat leider auch an Qualität verloren nach dem rendern...



Ich würde mich aber trotzdem sehr freuen, wenn du das Programm tatsächlich ausführen würdest und
nicht nur den Programm Code durchguckst. :)


Zu den Funktionen:
Zum Zeichen eines Polygons muss auf dem "Pen Tool" geklickt werden. Die Polygone auf dem Canvas werden
deselektiert, wenn nochmal auf dem Pen Tool geklickt wird oder wenn mit Rechtsklick auf dem Canvas
geklickt wird. Mit dem Radierer kann man einzelnen Punkte löschen, wenn man auf diese klickt.
Sind mehrere Polygone gezeichnet worden und möchte man zu einem von ihnen noch mehr Punkte hinzufügen
oder die Attribute wie Farbe, Strichdicke, Härte oder den Zeichenmodus (Line, Lineloop, Filled Polygon)
ändern, so klickt man einfach auf eines der gezeichneten Polygon und fürht die gewünschten Einstellungen
durch. Dasselbe gilt auch für das T und die Rekursionstiefe der Subdivisions bzw. dem T beim "Smoothing".

Key Callbacks:
B - Zeigt die Bezier Kurve des selektierten Polygons an, beim nochmaligen Klicken wird diese wieder
	ausgeblendet

C - Zeigt die Kubische Spline Kurve des selektierten Polygons an, beim nochmaligen Klicken wird diese 
	wieder ausgeblendet

I - Zeigt die Kubische Interpoaltions Kurve des selektierten Polygons an, beim nochmaligen Klicken
	wird diese wieder ausgeblendet

Q - Zeigt die Quadratische Spline Kurve des selektierten Polygons an, beim nochmaligen Klicken wird
	diese wieder ausgeblendet

P - Blendet das Ursprungspolygon aus bzw. ein beim nochmaligen klicken

S - Macht das Polygon "weicher"

Entf - Löscht das ausgewählte Polygon mit samt den Subdivions



Buttons:

Pen Tool - zeichnet ein Polygon

Radierer - löscht einen angeklickten Punkt (beim nochmaligen Klicken auf dem Radierer Button wird dieser
	ausgeschaltet)

Härte/Weichzeichnung des Polygons - zeichnet die Linien des Polygons und der Subdivisions hart oder
	weich, es ist immer ein Status ausgewählt

Line/Line Loop/Filled Polygon - zeichnet das Polygon und die Subdivisions des Polygons im gewünschten
	Modus, es ist immer ein Modus ausgewählt

Farbauswahl - setzt die Farbe des ausgewählten Polygons und dessen Subdivions auf die gewünschte Farbe
	man kann die Farbe präziser auswählen, wenn man die Maus gedrückt hält




Slider:

Line Width - macht die Linienstärke des gewählten Polygons dicker oder dünner (reicht von 1 bis 10)
	Default Wert: 1

Grayscale - macht die Farbwahl dunkler bzw. heller (reicht von 0 bis 1)
	Default Wert: 1

T (Bezier Subdivision, Quadratic Spline, Cubic Spline Cubic Interpolation, Smooth) - verändert die
	Koeffizienten in der Maske, die Default Einstellungen sind so, wie du sie in deinen Vorlesungen
	hattest, beim setzten eines T's bei den Algorithmen, habe ich darauf geachten, dass alle
	Monokoeffizienten zusammen immer 1 ergeben (reicht von 0 bis 1)
	Default Wert Bezier: 0.5
	Default Wert Splines, Interpolation: 0.25
	Default Wert Smooth: 0.33

Depth (Bezier Subdivision, Quadratic Spline, Cubic Spline Cubic Interpolation) - verändert die
	Rekursionstiefe der Algorithmen. (reicht von 1 bis 5 [größer ging nicht, da die Berechnung
	dann zu lange dauert bei der Bezier Kurve, du kannst den maximalen Wert aber selber in der
	Datenbank umändern, musst im Code nur vorher gucken welcher der Werte den maximalen Wert
	anzeigt - steht in der 'loadSlider' Methode des SliderModels])
	Default Wert bei allen: 3

Da ich keinen Text rendern konnte bzw. das für mich zu lange gedauert hätte, werden die Werte der
Slider in der Konsole angezeigt.


Alle genannten Einstellungen (Linienhärte, Zeichenmodus, Farbe, Linienbreite, T, Depth, Smooth) gelten
nicht global. Du musst immer ein Polygon auswählen, das du ändern möchtest. Dann wird auch nur dieses
geändert. Zum Deselektieren des Polyogns einfach wieder auf dem Canvas klicken.
Dasselbe gilt auch für die Key Callbacks.



Die Algorithmen für das Berechnen der Subdivisions findest du in "GraphicObjects.cpp" in der Klasse
"Polygon". 



(c) Noshaba Cheema 2014