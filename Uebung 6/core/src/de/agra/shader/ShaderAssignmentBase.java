package de.agra.shader;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Mesh;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.VertexAttribute;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Vector3;

import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

public class ShaderAssignmentBase extends ApplicationAdapter {
    SpriteBatch batch;
    Texture img;
    public PerspectiveCamera cam;
    public ShaderProgram shader;

    public float time = 0; //Zeit-Variable für Aufgabe 2-Wabern
    private float timeIncrementPerStep = 0.01f;

    /**
     * Wir nutzen statt einzelnen Variablen für jedes Mesh ein Mesh-Array, das bequem durchiteriert werden kann.
     * Die Variable war zuvor public- war das nötig?
     */
    public Mesh[] meshList = new Mesh[3];

    /**
     * X-Offset aller Meshes.
     * Wird genutzt, um die Meshes zu zentrieren und auf der X-Achse beliebig verschiebbar zu machen.
     */
    private final float meshStartOffX = -1.2f;

    /**
     * X-Offset zwischen einzelnen Meshes.
     * Wird mit der Nummer der aktuellen Meshes multipliziert, um Formen nebeneinander darzustellen.
     */
    private final float meshOffX = java.lang.Math.abs(meshStartOffX);



    @Override
    public void create() {
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
     * Gibt ein Mesh zurück, dessen Vertices und Indices bereits definiert wurden.
     *
     * @param typeNumber - Pyramide (0), Zylinder (1) oder Kugel (2)?
     *                   Bei allen anderen Zahlen wird stattdessen das vorgegebene Logo gezeigt.
     * @param meshNumber - Der wievielte gezeigte Mesh ist dies?
     *                   Wird zum Offset der Mesh-Koordinaten genutzt.
     */
    private Mesh getMesh(int typeNumber, int meshNumber) {
        switch (typeNumber) {
            case 1:
                return getMeshCylinder(meshNumber);
            default:
                return getMeshLogo(meshNumber);
        }
    }

    /**
     * Generiert ein Logo. Vorgegeben!
     * @param meshNumber
     * @return Das Logo als Mesh.
     */
    private Mesh getMeshLogo(int meshNumber) {
        //Gesamter x-offset
        float xm = meshStartOffX + meshNumber * meshOffX;

        Mesh mesh;

        mesh = new Mesh(true, 4, 6, VertexAttribute.Position(), VertexAttribute.ColorUnpacked(), VertexAttribute.TexCoords(0));

        //Mesh generieren:
        mesh.setVertices(new float[]
                // Position XYZ   Color RGBA   Texture Coordinates UV
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
     * @param meshNumber
     * @return Das Pyramidenmesh.
     */
    private Mesh getMeshPyramid(int meshNumber) {
        return getMeshLogo(meshNumber);
    }

    /**
     * Generiert ein Zylindermesh..
     * @param meshNumber
     * @return Das Zylindermesh..
     */
    private Mesh getMeshCylinder(int meshNumber) {

        /**
         * Idee:
         * Einen Kreis generieren.
         * Dann einen weiteren Kreis mit höheren y-Koordinaten erstellen und mit vorherigem Kreis verbinden.
         */

        //Zylinderattribute
        final int cylinderFaces = 20; //Mindestens 3
        final float cylinderHeight = 2f;
        final float cylinderRadius = 0.5f;


        //Gesamter x-offset
        float xm = meshStartOffX + meshNumber * meshOffX;

        //Flächen erstellen.


        //Größe: (Punkte * Flächen + 2 Mittelpunkte) * 9 Attribute eines Vektors
        float [] vertices = new float[(cylinderFaces*2+2)*7];


        //Beim zweiten Durchlauf wird i zu 1 und bewirkt, dass statt Fläche 1 die Fläche 2 erstellt wird.
        for (int isTop = 0; isTop<=1; isTop++){
            for(int j = 0; j<cylinderFaces; j++){ //+1 für den Mittelpunkt!

                //Modifiziertes j.
                //j*9 Werte pro Punkt + i * Anzahl der Vertices-Einträge der ersten Fläche.
                int jMod = j*7+(isTop*cylinderFaces*7);

                vertices[jMod] = ShapeGen.getCirclePointX(cylinderRadius, ((float)j/(float)cylinderFaces)*360) + xm; //x
                vertices[jMod+1] = ShapeGen.getCirclePointY(cylinderRadius, ((float)j/(float)cylinderFaces)*360);  //y


                vertices[jMod+2] = cylinderHeight/2 - (isTop * cylinderHeight); //z
                vertices[jMod+3] = 1;    //r
                vertices[jMod+4] = 1;    //b
                vertices[jMod+5] = 1;    //g
                vertices[jMod+6] = 1;    //a
            }
        }

        //TODO Deckel generieren

        Mesh mesh;

        int maxVertices = cylinderFaces*2+2;
        int maxIndices = cylinderFaces * 2 * 3 + cylinderFaces * 3;

        mesh = new Mesh(true, maxVertices, maxIndices, VertexAttribute.Position(), VertexAttribute.ColorUnpacked());

        //Vertices festlegen:
        mesh.setVertices(vertices);

        //Nun müssen die Indices festgelegt werden!

        short [] indices = new short[cylinderFaces*2*3]; //TODO beachtet noch nicht mittelpunkte

        for (int i = 0; i<cylinderFaces; i++){

            //TODO korrigieren
            //TODO Deckel verbinden
            indices[i*6] = (short)i;
            indices[i*6+1] = (short)((i+1)%cylinderFaces);
            indices[i*6+2] = (short)(i+cylinderFaces);

            indices[i*6+3] = indices[i*6+1];
            indices[i*6+4] = indices[i*6+2];
            indices[i*6+5] = (short)((i+1)%cylinderFaces + cylinderFaces);
        }

        System.out.println(Arrays.toString(indices));

        //Platzhalter-Indizes:
        mesh.setIndices(indices);

        return mesh;
    }

    /**
     * Generiert ein Kugelmesh..
     * @param meshNumber
     * @return Das Kugelmesh..
     */
    private Mesh getMeshSphere(int meshNumber) {
        return getMeshLogo(meshNumber);
    }

    @Override
    public void render() {
        Gdx.gl.glClearColor(0.25f, 0.25f, 0.5f, 1);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT | GL20.GL_DEPTH_BUFFER_BIT);
        Gdx.gl.glEnable(GL20.GL_DEPTH_TEST);

        cam.position.set(
                (Gdx.input.getX() - Gdx.graphics.getWidth() / 2f) / 100f,
                (Gdx.input.getY() - Gdx.graphics.getHeight() / 2f) / 100f,
                10f);
        cam.up.set(0, 1, 0);
        cam.lookAt(0, 0, 0);
        cam.update();

        img.bind();
        shader.begin();
        shader.setUniformMatrix("u_worldView", cam.combined);
        shader.setUniformi("u_texture", 0);


        for (Mesh aMeshList : meshList) aMeshList.render(shader, GL20.GL_TRIANGLES);
        //mesh.
        shader.end();
    }

    @Override
    public void resize(int width, int height) {
        cam.viewportHeight = Gdx.graphics.getHeight();
        cam.viewportWidth = Gdx.graphics.getWidth();
    }

    private float increaseTime(){
        time+=timeIncrementPerStep;
        //Da wir Sinusfunktionen nutzen, muss Time nie über 1 hinausgehen.
        if(time>=1){
            time=0;
        }
        System.out.println(Float.toString(time));
        //Wir sparen uns den späteren Abruf der Variable, wenn wir ihn einfach direkt zurückgeben.
        return time;
    }
}
