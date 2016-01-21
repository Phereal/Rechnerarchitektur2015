package de.agra.shader;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.Color;
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
            case 0:
                return getMeshPyramid(meshNumber);
            case 1:
                return getMeshCylinder(meshNumber);
            case 2:
                return getMeshSphere(meshNumber);
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

    	float xm = meshStartOffX + meshNumber * meshOffX;

    	float pos = 0.5f;
    	float neg = -0.5f;
    	float zer = 0f;

    	float[] vertices = {
    			neg + xm, neg, neg, 1, 1, 1, 1,	//0
                pos + xm, neg, neg, 1, 1, 1, 1,	//1
                pos + xm, neg, pos, 1, 1, 1, 1,	//2
                neg + xm, neg, pos, 1, 1, 1, 1,	//3
                zer + xm, pos, zer, 1, 1, 1, 1	//4
    	};

    	short[] indices = {
    			0, 1, 2,		//Grundplatte Teil 1
    			2, 3, 0,		//Grundplatte Teil 2
    			0, 4, 1,		//vordere Seite
    			1, 4, 2,		//rechte Seite
    			2, 4, 3,		//hintere Seite
    			3, 4, 0			//linke Seite
    	};

    	Mesh mesh;

    	mesh = new Mesh(true, vertices.length/7, indices.length, VertexAttribute.Position(), VertexAttribute.ColorUnpacked());
    	mesh.setVertices(vertices);
    	mesh.setIndices(indices);

        return mesh;
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
        final float cylinderHeight = 1.6f;
        final float cylinderRadius = 0.5f;
        final int cylinderRings = 10;


        //Gesamter x-offset
        float xm = meshStartOffX + meshNumber * meshOffX;

        //Flächen erstellen.


        //Größe: (Punkte * Flächen + 2 Mittelpunkte) * 9 Attribute eines Vektors
        float [] vertices = new float[(cylinderFaces*cylinderRings+2)*7];

         //Röhrenpunkte generieren
        for (int i = 0; i<cylinderRings; i++){
            for(int j = 0; j<cylinderFaces; j++){ //+1 für den Mittelpunkt!

                //Modifiziertes j.
                //j*9 Werte pro Punkt + i * Anzahl der Vertices-Einträge der ersten Fläche.
                int jMod = j*7+(i*cylinderFaces*7);

                vertices[jMod] = ShapeGen.getCirclePointX(cylinderRadius, ((float)j/(float)cylinderFaces)*360) + xm; //x
                vertices[jMod+1] = -((float)(cylinderHeight/2)) + ((((float)i)/(cylinderRings-1)) * cylinderHeight);  //y
                vertices[jMod+2] = ShapeGen.getCirclePointZ(cylinderRadius, ((float)j/(float)cylinderFaces)*360); //z

                vertices[jMod+3] = 0;    //r
                vertices[jMod+4] = ((i/(float)cylinderRings))%cylinderRings;    //b
                vertices[jMod+5] = ((i/(float)cylinderRings)+1)%cylinderRings;    //g
                vertices[jMod+6] = ((i/(float)cylinderRings)+2)%cylinderRings;    //a
            }
        }



        Mesh mesh;

        int maxVertices = (cylinderFaces*cylinderRings)+2;

        //Flächen * Ringe * 3 (Dreieck) * 2 (Viereck) + Flächen * 3 (Dreieck) * 2 (je Deckel & Boden))
        int maxIndices = cylinderFaces * cylinderRings * 3 *2 + cylinderFaces * 3 * 2;

        mesh = new Mesh(true, maxVertices, maxIndices, VertexAttribute.Position(), VertexAttribute.ColorUnpacked());

        //Vertices festlegen:
        mesh.setVertices(vertices);

        //Nun müssen die Indices festgelegt werden!

        short [] indices = new short[3*cylinderFaces*2*(cylinderRings-1)]; //TODO beachtet noch nicht mittelpunkte

        //Röhre verbinden
        for(int i = 0; i<cylinderRings-1;i++){ //-1 da die oberste Schicht folgenden Code nicht ausführen darf

            int iOff = i * cylinderFaces; //Layer-Offset

            for (int j = 0; j<cylinderFaces; j++){
                //TODO Deckel verbinden

                System.out.println(Integer.toString(i));

                //Dreieck 1
                indices[(j+iOff)*6   ] = (short)(j+iOff);
                indices[(j+iOff)*6 +1] = (short)(((j+1)%cylinderFaces)+iOff);
                indices[(j+iOff)*6 +2] = (short)(j+cylinderFaces+iOff);

                //Dreieck 2
                indices[(j+iOff)*6 +3] = indices[(j+iOff)*6 +1];
                indices[(j+iOff)*6 +4] = indices[(j+iOff)*6 +2];
                indices[(j+iOff)*6 +5] = (short)(((j+1)%cylinderFaces + cylinderFaces)+iOff);
            }
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
        Gdx.gl.glClearColor(0, 0, 0, 0);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT | GL20.GL_DEPTH_BUFFER_BIT);
        Gdx.gl.glEnable(GL20.GL_DEPTH_TEST);

        cam.position.set(
                (Gdx.input.getX()*6 - (Gdx.graphics.getWidth() / 2f)*6) / 100f,
                (Gdx.input.getY()*6 - (Gdx.graphics.getHeight() / 2f)*6) / 100f,
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
