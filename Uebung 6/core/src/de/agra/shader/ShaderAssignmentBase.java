package de.agra.shader;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.*;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;


import java.util.Arrays;

public class ShaderAssignmentBase extends ApplicationAdapter {

    SpriteBatch batch;
    Texture img;
    public PerspectiveCamera cam;
    public ShaderProgram shader;

    public float time = 0; //Zeit-Variable für Aufgabe 2-Wabern
    private float timeIncrementPerStep = 0.025f;

    /*
         * Wir nutzen statt einzelnen Variablen für jedes Mesh ein Mesh-Array, das bequem durchiteriert werden kann.
         * Die Variable war zuvor public- war das nötig?
     */
    public Mesh[] meshList = new Mesh[3];

    /*
         * X-Offset aller Meshes.
         * Wird genutzt, um die Meshes zu zentrieren und auf der X-Achse beliebig verschiebbar zu machen.
     */
    private final float meshStartOffX = -2f;

    /*
         * X-Offset zwischen einzelnen Meshes.
         * Wird mit der Nummer der aktuellen Meshes multipliziert, um Formen nebeneinander darzustellen.
     */
    private final float meshOffX = java.lang.Math.abs(meshStartOffX);

    //Pyramidenattribute
    final int pyramidLayers = 5;
    final float pyramidWidth = 1;
    final float pyramidHeight = 2;
    final int pyramidMeshComponents = 10;

    //Zylinderattribute
    final int cylinderFaces = 50;
    final float cylinderHeight = 1.6f;
    final float cylinderRadius = 0.5f;
    final int cylinderRings = 100;
    final int cylinderMeshComponents = 10;

    //Kugelattribute
    final int thetaIntervalle = 20;
    final int phiIntervalle = 20;
    final float sphereRadius = 0.75f;
    final int sphereMeshComponents = 10; // 3x Pos, 4x RGBA, 3x Norm

    @Override
    public void create() {
        System.out.println(Arrays.asList("Los geht's."));

        batch = new SpriteBatch();
        img = new Texture("agra.png");
        FileHandle vertexShaderH = new FileHandle("vertexShader.prg");
        FileHandle fragmentShaderH = new FileHandle("fragmentShader.prg");

        shader = new ShaderProgram(vertexShaderH, fragmentShaderH);

        //Fülle die Mesh-Liste mit den gewünschten Formen.
        for (int i = 0; i < meshList.length; i++) {
            meshList[i] = getMesh(i, i);
        }

        cam = new PerspectiveCamera(20, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        cam.position.set(0f, 0f, 10f);
        cam.lookAt(0, 0, 0);
        cam.near = 1f;
        cam.far = 300f;
        cam.update();
    }

    /**
     * Gibt ein Mesh zurück, dessen Vertices und Indices bereits definiert
     * wurden.
     *
     * @param typeNumber - Pyramide (0), Zylinder (1) oder Kugel (2)? Bei allen
     *                   anderen Zahlen wird stattdessen das vorgegebene Logo gezeigt.
     * @param meshNumber - Der wievielte gezeigte Mesh ist dies? Wird zum Offset
     *                   der Mesh-Koordinaten genutzt.
     */
    private Mesh getMesh(int typeNumber, int meshNumber) {
        switch (typeNumber) {
            case 0:
                return getMeshPyramid(meshNumber);
            case 1:
                return getMeshSphere(meshNumber);
            case 2:
                return getMeshCylinder(meshNumber);
            default:
                return getMeshLogo(meshNumber);
        }
    }

    /**
     * Generiert ein Logo. Vorgegeben!
     *
     * @param meshNumber
     * @return Das Logo als Mesh.
     */
    private Mesh getMeshLogo(int meshNumber) {
        //Gesamter x-offset
        float xm = meshStartOffX + meshNumber * meshOffX;

        Mesh mesh;

        mesh = new Mesh(true, 4, 6, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(), VertexAttribute.TexCoords(0));

        //Mesh generieren:
        mesh.setVertices(new float[] // Position XYZ   Color RGBA   Texture Coordinates UV
                //|------------|  |--------|  |--|
                {-0.5f + xm, -0.5f, 0, 1, 1, 1, 1, 0, 1,
                        0.5f + xm, -0.5f, 0, 1, 1, 1, 1, 1, 1,
                        0.5f + xm, 0.5f, 0, 1, 1, 1, 1, 1, 0,
                        -0.5f + xm, 0.5f, 0, 1, 1, 1, 1, 0, 0});

        mesh.setIndices(new short[]{0, 1, 2, 2, 3, 0});

        return mesh;
    }

    /**
     * Generiert ein Pyramidenmesh.
     *
     * @param meshNumber
     * @return Das Pyramidenmesh.
     */
    private Mesh getMeshPyramid(int meshNumber) {

        if (pyramidWidth < 0 || pyramidLayers < 1) {
            System.out.println("Diese Pyramide hat seltsame Parameter. Was. Zur. Hölle.");
        }

        float totalOffsetX = meshStartOffX + meshNumber * meshOffX;

        Mesh mesh;

        //Flächen einer Pyramidenseite mal vier + unterseite
        int maxVertices = (pyramidLayers * pyramidLayers) * 4 + pyramidLayers * 4;

        //Zentrierte Quadratzahl plus 1
        int maxIndices = (pyramidLayers * pyramidLayers) * 4 * pyramidMeshComponents + pyramidMeshComponents;


        mesh = new Mesh(true, maxVertices, maxIndices, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(),
                VertexAttribute.Normal());

        float[] vertices = getMeshPyramidVertices(totalOffsetX);
        mesh.setVertices(vertices);

        short[] indices = getMeshPyramidIndices();
        mesh.setIndices(indices);

        return mesh;
    }

    private float[] getMeshPyramidVertices(float totalOffsetX) {

        float[] vertices = new float[(pyramidLayers * pyramidLayers) * 4 * pyramidMeshComponents + pyramidMeshComponents];


        //Ebenen- und Seitenabhängiger Offset der Vertices-Indexierung
        int iOffset = 0;

        //Schichten
        for (int currentLayer = 0; currentLayer <= pyramidLayers; currentLayer++) {


            int pointsPerSide = (pyramidLayers - currentLayer);


            float triangleWidth = pyramidWidth / pointsPerSide;
            float triangleHeight = (float) (Math.sqrt((float) 3) / 2) * triangleWidth;


            //Ein Wert, der in der unsersten Schicht 1 beträgt, in der Mittleren 0.5 und in der obersten 0.
            //Alle XZ-Koordinaten mit diesem Wert multiplizieren
            float factorXYZ = 1 - (currentLayer) / (float)(pyramidLayers);


            //Jede Außenseite einer Schicht generieren
            for (int side = 0; side <= 3; side++) { //right, forward, left, back


                //Jede Vertex einer Seite einer Schicht generieren
                for (int i = 0; i < pointsPerSide; i++) { //Letzter Punkt gehört zur nächsten Seite

                    switch (side) {
                        case 0://rechts
                            vertices[iOffset] = totalOffsetX + (pyramidWidth / 2) * factorXYZ; //x
                            vertices[iOffset + 2] = (-pyramidWidth / 2 + i * triangleWidth) * factorXYZ;//z TODO wo war nochmal vorne..?

                            vertices[iOffset+7] =(float)0.707;
                            vertices[iOffset+8] =0;
                            vertices[iOffset+9] =(float)0.707;
                            break;
                        case 1://vorne
                            vertices[iOffset] = totalOffsetX + (pyramidWidth / 2 - i * triangleWidth) * factorXYZ; //x
                            vertices[iOffset + 2] = (pyramidWidth / 2) * factorXYZ;//z

                            vertices[iOffset+7] =(float)0.707;
                            vertices[iOffset+8] =0;
                            vertices[iOffset+9] =(float)0.707;
                            break;
                        case 2://links
                            vertices[iOffset] = totalOffsetX - (pyramidWidth / 2) * factorXYZ; //x
                            vertices[iOffset + 2] = (pyramidWidth / 2 - i * triangleWidth) * factorXYZ;//z

                            vertices[iOffset+7] =-(float)0.707;
                            vertices[iOffset+8] =0;
                            vertices[iOffset+9] =(float)0.707;
                            break;
                        case 3://hinten
                            vertices[iOffset] = totalOffsetX - (pyramidWidth / 2 - i * triangleWidth) * factorXYZ; //x
                            vertices[iOffset + 2] = -(pyramidWidth / 2) * factorXYZ;//z

                            vertices[iOffset+7] =0;
                            vertices[iOffset+8] =(float)0.707;
                            vertices[iOffset+9] =-(float)0.707;
                            break;


                    }
                    //Andere Attribute
                    vertices[iOffset + 1] = (float) -(pyramidHeight/2) + currentLayer * (pyramidHeight/pyramidLayers); //y
                    vertices[iOffset + 3] = (float)0.5;
                    vertices[iOffset + 4] = (float)0.5;
                    vertices[iOffset + 5] = (float)0.5;
                    vertices[iOffset + 6] = (float)0.5;

                    if (pointsPerSide > 0) {
                        iOffset += pyramidMeshComponents; //Nächster Eckpunkt
                    }



                }


            }
        }

        //Spitze
        vertices[iOffset]     = totalOffsetX; //x
        vertices[iOffset + 1] = pyramidHeight / 2; //y
        vertices[iOffset + 2] = 0; //z
        vertices[iOffset + 3] = (float)0.5;
        vertices[iOffset + 4] = (float)0.5;
        vertices[iOffset + 5] = (float)0.5;
        vertices[iOffset + 6] = (float)0.5;
        vertices[iOffset + 7] =0;
        vertices[iOffset + 8] =1;
        vertices[iOffset + 9] =0;

        System.out.println("Pyramiden-Vertices berechnet.");
        return vertices;
    }

    /**
     * Generiert Indizes des Pyramidenmesh
     *
     * @return Indexliste
     */
    private short[] getMeshPyramidIndices() {

        //Zentrierte Quadratzahl plus 1
        short[] indices = new short[(pyramidLayers * pyramidLayers) * 4 * pyramidMeshComponents + pyramidMeshComponents];


        for (int currentLayer = 0; currentLayer < pyramidLayers - 1; currentLayer++) {
            System.out.println("Layer "+Integer.toString(currentLayer));

            int pointsPerSide = (pyramidLayers - currentLayer);
            int pointsPerLayer = pointsPerSide * 4;
            int pointsNextLayer = (pyramidLayers - currentLayer -1)*4;
            int iOffset;

            if(currentLayer == 0){
                iOffset = 0;
            }
            else{
                iOffset= (int) (Math.pow(pyramidLayers+1,2) + Math.pow(pyramidLayers-1+1,2))
                        - (int) (Math.pow(pyramidLayers+1-currentLayer,2) + Math.pow(pyramidLayers-currentLayer-1+1,2));
            }




            int nextLevelOffset = 0;
            for (int side = 0; side < 4; side++){
                System.out.println("    Side "+Integer.toString(side));
                for (int i = 0 + side * pointsPerSide; i < (side+1) * pointsPerSide; i++) {
                    indices[iOffset*4 + i*3]    = (short) (i + iOffset);
                    indices[iOffset*4 + i*3 + 1]= (short) (((i + 1)%pointsPerLayer) + iOffset);
                    indices[iOffset*4 + i*3 + 2]= (short) ((i+nextLevelOffset)%pointsNextLayer+pointsPerLayer+iOffset);

                    System.out.println("        i  "+Short.toString((short) iOffset));
                }
                nextLevelOffset-=1;
            }

        }
        System.out.println(Arrays.toString(indices));
        return indices;
    }

    /**
     * Generiert ein Zylindermesh..
     *
     * @param meshNumber
     * @return Das Zylindermesh..
     */
    private Mesh getMeshCylinder(int meshNumber) {

        //Bei blöden Parametern Warnung aussprucken.
        if (cylinderFaces < 3 || cylinderHeight <= 0 || cylinderRadius <= 0 || cylinderRings < 2) {
            System.out.println("Komische Zylinder-Paramter. Wahrscheinlich passiert jetzt etwas seltsames.");
        }

        //Gesamter x-offset
        float totalOffsetX = meshStartOffX + meshNumber * meshOffX;

        Mesh mesh;
        int maxVertices = (cylinderFaces * cylinderRings) + 2;
        //Flächen * Ringe * 3 (Dreieck) * 2 (Viereck) + Flächen * 3 (Dreieck) * 2 (je Deckel & Boden))
        int maxIndices = cylinderFaces * cylinderRings * 3 * 2 + cylinderFaces * 3 * 2;
        mesh = new Mesh(true, maxVertices, maxIndices, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(),
                VertexAttribute.Normal());

        float[] vertices = getMeshCylinderVertices(totalOffsetX);
        mesh.setVertices(vertices);

        short[] indices = getMeshCylinderIndices();
        mesh.setIndices(indices);


        return mesh;
    }

    /**
     * Generiert die Zylinders-Vertices.
     *
     * @param totalOffsetX - Gesamtverschiebung / Position des Zylinders
     * @return Alle Vertices als float-Array.
     */
    private float[] getMeshCylinderVertices(float totalOffsetX) {
        //Gesamter x-offset
        float xm = totalOffsetX;
        //Größe: (Punkte * Flächen + 2 Mittelpunkte) * 9 Attribute eines Vektors
        float[] vertices = new float[(cylinderFaces * cylinderRings + 2) * cylinderMeshComponents];
        //Röhrenpunkte generieren
        for (int i = 0; i < cylinderRings; i++) {
            for (int j = 0; j < cylinderFaces; j++) { //+1 für den Mittelpunkt!

                //Modifiziertes j.
                //j*9 Werte pro Punkt + i * Anzahl der Vertices-Einträge der ersten Fläche.
                int jMod = j * cylinderMeshComponents + (i * cylinderFaces * cylinderMeshComponents);

                vertices[jMod] = ShapeGen.getCirclePointX(cylinderRadius, ((float) j / (float) cylinderFaces) * 360) + xm; //x
                vertices[jMod + 1] = -((float) (cylinderHeight / 2)) + ((((float) i) / (cylinderRings - 1)) * cylinderHeight);  //y
                vertices[jMod + 2] = ShapeGen.getCirclePointZ(cylinderRadius, ((float) j / (float) cylinderFaces) * 360); //z

                vertices[jMod + 3] = (((float) i / cylinderRings * 3) % 3) / 2;    //r
                vertices[jMod + 4] = (((float) i / cylinderRings * 3 + 1) % 3) / 2;    //b
                vertices[jMod + 5] = (((float) i / cylinderRings * 3 + 2) % 3) / 2;    //g
                vertices[jMod + 6] = 1;                     //a

                float vectorLength = (float) Math.sqrt(((vertices[jMod] - xm) * (vertices[jMod] - xm)
                        + (vertices[jMod + 1]) * (vertices[jMod + 1])
                        + (vertices[jMod + 2]) * (vertices[jMod + 2])));


                vertices[jMod + 7] = (1 / vectorLength) * (vertices[jMod] - xm);
                vertices[jMod + 8] = 0;
                vertices[jMod + 9] = (1 / vectorLength) * (vertices[jMod + 2]);

            }
        }

        //Boden-Mittelpunkt
        vertices[vertices.length - cylinderMeshComponents * 2] = 0 + xm;
        vertices[vertices.length - cylinderMeshComponents * 2 + 1] = vertices[1]; //Wird immer die richtige Höhe beinhalten!
        vertices[vertices.length - cylinderMeshComponents * 2 + 2] = 0; //Nun: Farbe von unterstem Ring klauen
        vertices[vertices.length - cylinderMeshComponents * 2 + 3] = vertices[3];
        vertices[vertices.length - cylinderMeshComponents * 2 + 4] = vertices[4];
        vertices[vertices.length - cylinderMeshComponents * 2 + 5] = vertices[5];
        vertices[vertices.length - cylinderMeshComponents * 2 + 6] = 1;

        //Decken-Mittelpunkt
        vertices[vertices.length - cylinderMeshComponents] = 0 + xm;
        vertices[vertices.length - cylinderMeshComponents + 1] = vertices[vertices.length - cylinderMeshComponents * 3 + 1];
        vertices[vertices.length - cylinderMeshComponents + 2] = 0; //nun: Farbe von obersten Ring klauen
        vertices[vertices.length - cylinderMeshComponents + 3] = vertices[vertices.length - cylinderMeshComponents * 3 + 3];
        vertices[vertices.length - cylinderMeshComponents + 4] = vertices[vertices.length - cylinderMeshComponents * 3 + 4];
        vertices[vertices.length - cylinderMeshComponents + 5] = vertices[vertices.length - cylinderMeshComponents * 3 + 5];
        vertices[vertices.length - cylinderMeshComponents + 6] = 1;

        return vertices;
    }

    private short[] getMeshCylinderIndices() {
        //Randflächen + Deckel & Boden
        //3 (Dreieck) * Flächen * 2 (Viereck) * Ringe bis auf obersten + Flächen * 3 (Dreieck) * 2 (Deckel & Boden)
        short[] indices = new short[3 * cylinderFaces * 2 * (cylinderRings - 1) + cylinderFaces * 3 * 2];

        //Röhre verbinden
        for (int i = 0; i < cylinderRings - 1; i++) { //-1 da die oberste Schicht folgenden Code nicht ausführen darf

            int iOff = i * cylinderFaces; //Layer-Offset

            for (int j = 0; j < cylinderFaces; j++) {

                //Dreieck 1
                indices[(j + iOff) * 6] = (short) (j + iOff);
                indices[(j + iOff) * 6 + 1] = (short) (((j + 1) % cylinderFaces) + iOff);
                indices[(j + iOff) * 6 + 2] = (short) (j + cylinderFaces + iOff);

                //Dreieck 2
                indices[(j + iOff) * 6 + 3] = indices[(j + iOff) * 6 + 1];
                indices[(j + iOff) * 6 + 4] = indices[(j + iOff) * 6 + 2];
                indices[(j + iOff) * 6 + 5] = (short) (((j + 1) % cylinderFaces + cylinderFaces) + iOff);
            }
        }

        //Da wir nicht auf die Länge Zugriff haben, errechnen wir diese.
        //--
        //Boden:
        int verticesLength = (cylinderFaces * cylinderRings + 2);
        //Index, ab dem die Kappen beschrieben werden
        int capStartIndex = 3 * 2 * cylinderFaces * (cylinderRings - 1);
        for (int i = 0; i < cylinderFaces; i++) {
            indices[i * 3 + capStartIndex] = (short) i;
            indices[i * 3 + capStartIndex + 1] = (short) ((i + 1) % cylinderFaces);
            indices[i * 3 + capStartIndex + 2] = (short) (verticesLength - 2);
        }

        //--
        //Deckel:
        int topRingStartIndex = cylinderFaces * (cylinderRings - 1);
        capStartIndex = 3 * 2 * cylinderFaces * (cylinderRings - 1) + cylinderFaces * 3;
        for (int i = 0; i < cylinderFaces; i++) {
            indices[i * 3 + capStartIndex] = (short) (i + topRingStartIndex);
            indices[i * 3 + capStartIndex + 1] = (short) ((i + 1) % cylinderFaces + topRingStartIndex);
            indices[i * 3 + capStartIndex + 2] = (short) (verticesLength - 1);
        }


        return indices;
    }

    /**
     * Generiert ein Kugelmesh..
     *
     * @param meshNumber
     * @return Das Kugelmesh..
     */
    private Mesh getMeshSphere(int meshNumber) {

//    int thetaIntervalle = 4;
//    int phiIntervalle = 4;

        //Gesamter x-offset
        float totalOffsetX = meshStartOffX + meshNumber * meshOffX;

        // anzahl an vertices: je thetaIntervall 20 + ober- und untervertice
        int verticesCnt = (phiIntervalle * (thetaIntervalle - 1)) + 2;
        // anzahl an indices: 3 je Dreieck, 2 Dreiecke je innere Flaeche, 1 Dreieck je ober- unterflaeche
        int indicesCnt = ((phiIntervalle * (thetaIntervalle - 2) * 2) + (phiIntervalle * 2)) * 3;

        // Debug print
        //System.out.println("\nDebug: getMeshSphere()");
        //System.out.println("vertices: " + verticesCnt);
        //System.out.println("indices: " + indicesCnt);

        Mesh mesh;
        mesh = new Mesh(true, verticesCnt, indicesCnt, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(), VertexAttribute.Normal());

        float[] vertices = getMeshSphereVertices(totalOffsetX);
        mesh.setVertices(vertices);

        short[] indices = getMeshSphereIndices();
        mesh.setIndices(indices);

        return mesh;
//    return getMeshLogo(meshNumber);
    }


  private float[] getMeshSphereVertices(float totalOffsetX)
  {
    //Gesamter x-offset (lesebarer)
    float xm = totalOffsetX;

    int thetaStart = 0;
    int thetaStop = 180;
    int thetaRange = thetaStop - thetaStart;
    int thetaIntervallSize = thetaRange / thetaIntervalle;

    int phiStart = -180;
    int phiStop = 180;
    int phiRange = phiStop - phiStart;
    int phiIntervallSize = phiRange / phiIntervalle;

    // anzahl an vertices: je thetaIntervall 20 + ober- und untervertice
    int verticesCnt = (phiIntervalle * (thetaIntervalle - 1)) + 2;
    // anzahl an indices: 3 je Dreieck, 2 Dreiecke je innere Flaeche, 1 Dreieck je ober- unterflaeche
    int indicesCnt = ((phiIntervalle * (thetaIntervalle - 2) * 2) + (phiIntervalle * 2)) * 3;

    // Debug print
    System.out.println("\nDebug: getMeshSphereVertices()");
    System.out.println("vertices: " + verticesCnt);
    System.out.println("indices: " + indicesCnt);

    // haelt die vertices
    float[] vertices = new float[verticesCnt * sphereMeshComponents];
    // Index zum Zugriff aufs vertices-array
    int vId = 0;

    // y-ebene (ohne oberen und unteren punkt)
    for (int theta = (thetaStart + thetaIntervallSize); theta < thetaStop; theta += thetaIntervallSize)
    {
      // xz-ebene
      for (int phi = phiStart; phi < phiStop; phi += phiIntervallSize)
      {
        // Set x,y,z positionen
        vertices[vId++] = ShapeGen.getSphereKarthX(sphereRadius, (float) theta, (float) phi) + xm;
        vertices[vId++] = ShapeGen.getSphereKarthY(sphereRadius, (float) theta, (float) phi);
        vertices[vId++] = ShapeGen.getSphereKarthZ(sphereRadius, (float) theta, (float) phi);

        // Set RGBA values (Farbe ist bewusst umgekehrt zum Zylinder gewaehlt)
        vertices[vId++] = (((float) theta / thetaRange * 3) % 3) / 2;    //r
        vertices[vId++] = (((float) theta / thetaRange * 3 + 1) % 3) / 2;    //b
        vertices[vId++] = (((float) theta / thetaRange * 3 + 2) % 3) / 2;    //g
        vertices[vId++] = 1;

        // Set norm (-8 because of the vId++, otherwise it would be -7)
        vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8] - xm);
        vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);
        vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);
      }
    }

    //Top-Mittelpunkt
    vertices[vId++] = 0.0f + xm;
    vertices[vId++] = 0.0f;
    vertices[vId++] = sphereRadius;
    vertices[vId++] = vertices[3];
    vertices[vId++] = vertices[4];
    vertices[vId++] = vertices[5];
    vertices[vId++] = 1;
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8] - xm);
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);

    //Boden-Mittelpunkt
    vertices[vId++] = 0.0f + xm;
    vertices[vId++] = 0.0f;
    vertices[vId++] = -1.0f * sphereRadius;
    vertices[vId++] = vertices[vertices.length - sphereMeshComponents * 3 + 3];
    vertices[vId++] = vertices[vertices.length - sphereMeshComponents * 3 + 4];
    vertices[vId++] = vertices[vertices.length - sphereMeshComponents * 3 + 5];
    vertices[vId++] = 1;
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8] - xm);
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);
    vertices[vId++] = (1.0f / sphereRadius) * (vertices[vId - 8]);

    // Debug Print
    for (int i = 0; i < vertices.length; i += sphereMeshComponents)
    {
      //System.out.print("" + (i / sphereMeshComponents) + ": (" + (Math.round(vertices[i] * 100) / 100.0) + ", " + (Math.round(vertices[i + 1] * 100) / 100.0) + ", " + (Math.round(vertices[i + 2] * 100) / 100.0) + ")");
      //System.out.println(" (" + (Math.round(vertices[i+7] * 100) / 100.0) + ", " + (Math.round(vertices[i+8] * 100) / 100.0) + ", " + (Math.round(vertices[i+9] * 100) / 100.0) + ")");
    }

    return vertices;
  }

  private short[] getMeshSphereIndices()
  {

        // anzahl an vertices: je thetaIntervall 20 + ober- und untervertice
        int verticesCnt = (phiIntervalle * (thetaIntervalle - 1)) + 2;
        // anzahl an indices: 3 je Dreieck, 2 Dreiecke je innere Flaeche, 1 Dreieck je ober- unterflaeche
        int indicesCnt = ((phiIntervalle * (thetaIntervalle - 2) * 2) + (phiIntervalle * 2)) * 3;

        // Debug print
        //System.out.println("\nDebug: getMeshSphereIndices()");
        //System.out.println("vertices: " + verticesCnt);
        //System.out.println("indices: " + indicesCnt);

        short[] indices = new short[indicesCnt];
        for (int i = 0; i < indicesCnt; ++i) {
            indices[i] = 0;
        }
        // Index zum Zugriff aufs indices-array
        int iId = 0;

        for (int i = 0; i < (thetaIntervalle - 2); ++i) {
            int iOff = phiIntervalle * i;

            // xz-ebene
            for (int j = 0; j < phiIntervalle; ++j) {
                //Dreieck 1
                indices[iId++] = (short) (j + iOff);
                indices[iId++] = (short) (((j + 1) % phiIntervalle) + iOff);
                indices[iId++] = (short) (j + phiIntervalle + iOff);

                //Dreieck 2
                indices[iId++] = indices[(j + iOff) * 6 + 1];
                indices[iId++] = indices[(j + iOff) * 6 + 2];
                indices[iId++] = (short) (((j + 1) % phiIntervalle + phiIntervalle) + iOff);

//        indices[iId++] = (short) (j + 1 + iOff);
//        indices[iId++] = (short) (j + phiIntervalle + iOff);
//        if( j < (phiIntervalle-1) )
//        {
//          indices[iId++] = (short) (j + phiIntervalle + 1);
//        }
//        else
//        {
//          indices[iId++] = (short) (iOff);
//        }


//        indices[iId++] = (short) (((j + 1) % phiIntervalle)+ phiIntervalle );

//        //Dreieck 1
//        indices[(j + iOff) * 6] = (short) (j + iOff);
//        indices[(j + iOff) * 6 + 1] = (short) (((j + 1) % phiIntervalle) + iOff);
//        indices[(j + iOff) * 6 + 2] = (short) (j + phiIntervalle + iOff);
//
//        //Dreieck 2
//        indices[(j + iOff) * 6 + 3] = indices[(j + iOff) * 6 + 1];
//        indices[(j + iOff) * 6 + 4] = indices[(j + iOff) * 6 + 2];
//        indices[(j + iOff) * 6 + 5] = (short) (((j + 1) % phiIntervalle + phiIntervalle) + iOff);
            }
        }

        for (int i = 0; i < phiIntervalle; ++i) {
            indices[iId++] = (short) (i);
            indices[iId++] = (short) ((i + 1) % phiIntervalle);
            indices[iId++] = (short) (verticesCnt - 2);
        }

        for (int i = 0; i < phiIntervalle; ++i) {
            indices[iId++] = (short) ((verticesCnt - 3) - i);
            indices[iId++] = (short) ((verticesCnt - 3) - ((i + 1) % phiIntervalle));
            indices[iId++] = (short) (verticesCnt - 1);
        }


        //// Debug Print
        //for (int i = 0, j = 0; i < indicesCnt; i += 3) {
        //    if (j < 3) {
        //        System.out.print("(" + indices[i] + "," + indices[i + 1] + "," + indices[i + 2] + ") ");
        //        j++;
        //    } else {
        //        System.out.println("(" + indices[i] + "," + indices[i + 1] + "," + indices[i + 2] + ")");
        //        j = 0;
        //    }
        //}

        //System.out.println("");
        //System.out.println("Ende");

//    //Randflächen + Deckel & Boden
//    //3 (Dreieck) * Flächen * 2 (Viereck) * Ringe bis auf obersten + Flächen * 3 (Dreieck) * 2 (Deckel & Boden)
//    short[] indices = new short[3 * cylinderFaces * 2 * (cylinderRings - 1) + cylinderFaces * 3 * 2];
//
//    //Röhre verbinden
//    for (int i = 0; i < cylinderRings - 1; i++) { //-1 da die oberste Schicht folgenden Code nicht ausführen darf
//
//      int iOff = i * cylinderFaces; //Layer-Offset
//
//      for (int j = 0; j < cylinderFaces; j++) {
//
//        //Dreieck 1
//        indices[(j + iOff) * 6] = (short) (j + iOff);
//        indices[(j + iOff) * 6 + 1] = (short) (((j + 1) % cylinderFaces) + iOff);
//        indices[(j + iOff) * 6 + 2] = (short) (j + cylinderFaces + iOff);
//
//        //Dreieck 2
//        indices[(j + iOff) * 6 + 3] = indices[(j + iOff) * 6 + 1];
//        indices[(j + iOff) * 6 + 4] = indices[(j + iOff) * 6 + 2];
//        indices[(j + iOff) * 6 + 5] = (short) (((j + 1) % cylinderFaces + cylinderFaces) + iOff);
//      }
//    }
//
//    //Da wir nicht auf die Länge Zugriff haben, errechnen wir diese.
//    //--
//    //Boden:
//    int verticesLength = (cylinderFaces * cylinderRings + 2);
//    //Index, ab dem die Kappen beschrieben werden
//    int capStartIndex = 3 * 2 * cylinderFaces * (cylinderRings - 1);
//    for (int i = 0; i < cylinderFaces; i++) {
//      indices[i * 3 + capStartIndex] = (short) i;
//      indices[i * 3 + capStartIndex + 1] = (short) ((i + 1) % cylinderFaces);
//      indices[i * 3 + capStartIndex + 2] = (short) (verticesLength - 2);
//    }
//
//    //--
//    //Deckel:
//    int topRingStartIndex = cylinderFaces * (cylinderRings - 1);
//    capStartIndex = 3 * 2 * cylinderFaces * (cylinderRings - 1) + cylinderFaces * 3;
//    for (int i = 0; i < cylinderFaces; i++) {
//      indices[i * 3 + capStartIndex] = (short) (i + topRingStartIndex);
//      indices[i * 3 + capStartIndex + 1] = (short) ((i + 1) % cylinderFaces + topRingStartIndex);
//      indices[i * 3 + capStartIndex + 2] = (short) (verticesLength - 1);
//    }
//
//    System.out.println(Arrays.toString(indices));
//
        return indices;
    }

    @Override
    public void render() {
        Gdx.gl.glClearColor(0, 0, (float) 0.2, 0);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT | GL20.GL_DEPTH_BUFFER_BIT);
        Gdx.gl.glEnable(GL20.GL_DEPTH_TEST);

        cam.position.set(
                (Gdx.input.getX() * 6 - (Gdx.graphics.getWidth() / 2f) * 6) / 100f,
                (Gdx.input.getY() * 6 - (Gdx.graphics.getHeight() / 2f) * 6) / 100f,
                10f);
        cam.up.set(0, 1, 0);
        cam.lookAt(0, 0, 0);
        cam.update();

        img.bind();
        shader.begin();
        shader.setUniformi("u_texture", 0);
        shader.setUniformMatrix("u_worldView", cam.combined);
        shader.setUniformf("time", increaseTime());
        shader.setUniformf("v_camDirection", cam.direction);


        for (Mesh aMeshList : meshList) {
            aMeshList.render(shader, GL20.GL_TRIANGLES);
        }
        //mesh.
        shader.end();
    }

    @Override
    public void resize(int width, int height) {
        cam.viewportHeight = Gdx.graphics.getHeight();
        cam.viewportWidth = Gdx.graphics.getWidth();
    }

    private float increaseTime() {
        time += timeIncrementPerStep;
        //Da wir Sinusfunktionen nutzen, muss Time nie über 1 hinausgehen.
        if (time >= Math.PI * 2) {
            time = 0;
        }
        //Wir sparen uns den späteren Abruf der Variable, wenn wir ihn einfach direkt zurückgeben.
        return time;
    }

}
