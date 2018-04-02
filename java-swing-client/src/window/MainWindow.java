package window;

import java.awt.CardLayout;
import java.awt.Container;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import javax.swing.JFrame;

import socket.SocketOp;

public class MainWindow extends JFrame{
	Container 	container;
	CardLayout	card;

	FirPan firPan;
	public SecPan secPan;
	
	int 		xOld;
	int 		yOld;
	
	SocketOp socketOp;
	
	static boolean		status = true;
	MainWindow() {
		initMainWindow();
		while(true) {
			if(1 == firPan.conFlag) {
				this.socketOp = firPan.socketOp;;
				this.socketOp.setMainWindow(this);
				this.socketOp.initSocket();
				this.card.next(container);
				break;
			}
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	public void initMainWindow() {
		this.setBounds(200, 100, 800, 600);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setUndecorated(true);		
		this.setLayout(null);	
		
		card = new CardLayout(0, 3);
		container = this.getContentPane();
		container.setLayout(card);
		
		firPan = new FirPan(800, 600);
		secPan = new SecPan(800, 600);
		
		container.add(firPan);
		container.add(secPan);
		
		this.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				xOld = e.getX();
				yOld = e.getY();
			}
		});
		this.addMouseMotionListener(new MouseMotionAdapter() {
			@Override
			public void mouseDragged(MouseEvent e) {
				int xOnScreen = e.getXOnScreen();
				int yOnScreen = e.getYOnScreen();
				int xx = xOnScreen - xOld;
				int yy = yOnScreen - yOld;
				MainWindow.this.setLocation(xx, yy);
			}
		});
	
		this.setVisible(true);
	}
	public static void main(String[] args) {
		MainWindow mainWindow = new MainWindow();
	}
}
