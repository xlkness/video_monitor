package socket;

import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.awt.image.ImageProducer;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import window.MainWindow;

public class SocketOp {
	
	String ip;
	int port;
	Socket socket;
	InputStream inputStream;
	DataInputStream inputStreamData;
	PrintWriter outputStream;
	public int conStatus = 0;
	boolean status = true;
	MainWindow mainWindow;
	int clickCheck = -1;
	public SocketOp(Socket sock) {
		this.ip = ip;
		this.port = port;
		this.socket = sock;
	}
	public void initSocket() {
		
		try {
			//socket.connect(new InetSocketAddress(ip, port), 4000);
			conStatus = 1;
			inputStream = socket.getInputStream();
			inputStreamData = new DataInputStream(inputStream);
			outputStream = new PrintWriter(socket.getOutputStream());
			Thread th1 = new Thread() {
				public void run() {
					recv_task();
				}
			};
			th1.start();
			Thread th2 = new Thread() {
				public void run() {
					send_task();
				}
			};
			th2.start();
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	public void setMainWindow(MainWindow mainWindow) {
		this.mainWindow = mainWindow;
	}
	public void send_task() {
		outputStream.print("test");
		outputStream.flush();
		mainWindow.secPan.jlCap.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				if(2 != clickCheck) {
					outputStream.print("capture");
					outputStream.flush();  
					System.out.println("capture");
				}
					
			}
		}); 
		mainWindow.secPan.jlRecd.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				if(2 != clickCheck) {
					//µã»÷Â¼ÖÆ
					outputStream.print("record");
					outputStream.flush();
					mainWindow.secPan.jlRecd.setText("Í£ Ö¹");
					clickCheck = 2;
				}else if(2 == clickCheck) {
					outputStream.print("stoprecord");
					outputStream.flush();
					mainWindow.secPan.jlRecd.setText("Â¼ Ïñ");
					clickCheck = 1;
				}
				
			}
		});
	}
	public void recv_task() {
		while(status) {
			
			try {
				byte slen[] = new byte[5];
			
				inputStream.read(slen, 0, 5);
					
				String tmp = new String(slen);
				//System.out.println(tmp);
				int msglen = Integer.parseInt(tmp);
				
				//System.out.println(msglen);				
				
				byte msg[] = new byte[msglen];
				
				inputStreamData.readFully(msg);  
				
				ByteArrayInputStream imageStream = new ByteArrayInputStream(msg);  
				
	            BufferedImage bimage;
	            
				bimage = ImageIO.read(imageStream);
				
				ImageProducer producer = bimage.getSource();
            
	            Toolkit toolket = Toolkit.getDefaultToolkit();
	            
	            Image image = toolket.createImage(producer);
	            
	            ImageIcon ic = new ImageIcon(image);	            
	            
	            mainWindow.secPan.jlShow.setIcon(ic);
	            
	            mainWindow.secPan.jlShow.repaint();
			} catch (Exception e) {
				e.printStackTrace();
			}
            
            
		}
	}

}
